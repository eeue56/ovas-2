/*
 * File:   Analyser4D.cpp
 * Author: zoizoi
 *
 * Created on 23 January 2011, 17:51
 */

#include "Analyser4D.h"
#include "InfoData.h"
#include "PathVisualiser.h"
#include "ViewPathClusterFinder.h"

Analyser4D::Analyser4D() {
    //TemporalChangeFeature2 a;
    oc = new OVASControl();
    oc->geoSphere = new GeoSphere();
    oc->geoSequence = new GeoSequence(oc->geoSphere);
    oc->features = new vector<Feature*>();
    oc->viewEvaluator = new ViewEvaluator(oc);
    oc->pathVisualiser = new PathVisualiser(oc);
    // oc->volume4D=new Volume4D(oc);
    oc->a3d = new Analyser3D(oc);
    oc->filename = new string("");
    oc->volDataFileName = new string("none");
    oc->volActorActive = false;

    int startTime = time(NULL);
    char *command = new char[100];
    char* dir = new char[100];

    sprintf(command, "mkdir -p /home/zoizoi/psyforge/TVVVRunData/run%d", (int) startTime);
    system(command);
    sprintf(command, "mkdir -p /home/zoizoi/psyforge/TVVVRunData/run%d/codeDump", (int) startTime);
    system(command);
    sprintf(command, "cp *.cpp /home/zoizoi/psyforge/TVVVRunData/run%d/codeDump", (int) startTime);
    system(command);
    sprintf(command, "cp *.h /home/zoizoi/psyforge/TVVVRunData/run%d/codeDump", (int) startTime);
    system(command);
    sprintf(command, "cp *.txt /home/zoizoi/psyforge/TVVVRunData/run%d/codeDump", (int) startTime);
    system(command);
    sprintf(command, "cp *.config /home/zoizoi/psyforge/TVVVRunData/run%d/codeDump", (int) startTime);
    system(command);
    sprintf(command, "cp ./TVVV /home/zoizoi/psyforge/TVVVRunData/run%d/codeDump", (int) startTime);
    system(command);
    sprintf(dir, "/home/zoizoi/psyforge/TVVVRunData/run%d/", (int) startTime);
    oc->resultsPath = new string(dir);
}

Analyser4D::Analyser4D(const Analyser4D& orig) {
}

Analyser4D::~Analyser4D() {
}

void Analyser4D::init() {
    cout << "init with fn " << *oc->filename << endl;
    if (*oc->filename != "") {
        loadConfig(*oc->filename);
    }
    
    oc->renderXDim=300;
    oc->renderYDim=300;
    oc->a3d->init();

}

void Analyser4D::loadConfig(string filename) {
    cout << "\tloading config file " << filename << endl;

    ifstream inf;

    inf.open(filename.c_str());
    string line;

    while (getline(inf, line)) {

        if (line.length() != 0) {
            istringstream lineStream(line);
            string command;
            lineStream >> command;
            if (command == "iframes") {

                float startParam = 0, endParam = 0;

                lineStream >> startParam >> endParam >> numSteps;
                oc->stepToParamConverter = new StepToParamConverter(startParam, endParam, numSteps);
                cout << "\tiframes start " << startParam << " end " << endParam << " steps " << numSteps << endl;
                oc->numSteps = numSteps;
            }
            if (command == "new") {
                cout << " new" << endl;
                float wCurv, wtChange;
                string gsfilename;
                string screenRend;
                string showInterest;
                string ignoreAreaOnCriticalFrameStr;
                float wArea = 1;
                float wCompCount=0;
                float wbsize = 0; //.25;
                float wTop = 0; //.75;
                lineStream >> dims >> gsfilename >> screenRend >> wArea >> wbsize >> wTop >> wCurv >> wtChange>>wCompCount;

                gsfilename = "./sphereData/" + gsfilename;
                oc->geoSphere->loadGeoSphereFile(gsfilename);

                cout << " is " << oc->geoSphere->getNumVs() << endl;
                oc->features->push_back(new Feature(wArea, oc, "area"));
           

              
                if (screenRend == "onScreen") {
                    oc->viewEvaluator->setScreenRenderOn();
                }
                if (ignoreAreaOnCriticalFrameStr == "ignoreAreaOnCriticalFrame") {
                    
                    cout << "TODOignoreAreaOnCriticalFrame is On" << endl;
                } else {
                   
                }
                if (showInterest == "showInterest") {
                }
            }

           
            if (command == "flyingsaucers") {
                cout << "\tNew Flying Saucers " << dims << endl;
                //MetaballsVol4D*
                oc->numSteps = numSteps;
                oc->xDim = oc->yDim = oc->zDim = dims;
                oc->volume4D = dynamic_cast<Volume4D*> (new FlyingSaucersVol4D(oc, 4));
            }
          
        }
    }
}

void Analyser4D::analyse() {
    cout << "analysing all " << numSteps << "steps" << endl;
  

    for (int i = 0; i < numSteps; i++) {
        oc->volume4D->setToStep(i);
       oc->a3d->evalEachView();
        cout << " done step" << endl;
        oc->currentStep++;
    }
}

void Analyser4D::findOptimalPath() {

    InfoData* infoData = new InfoData(oc);
    oc->path = infoData->findOptimalPath();

}

void Analyser4D::findAndOutputPaths() {
    int numFeatures = oc->features->size();
    cout << numFeatures << " features" << endl;
    vector<Feature*>::iterator it;
    findOptimalPath();
    outputPath("change");
    outputBVs("bvs");
    outputPathVis("pathVis.png");

}


//We dont need this right now but leaving it in. (28/4/12)
void Analyser4D::findPathClusters() {
    
    int numberOfClusters=3;
    
    ViewPathClusterFinder* vpcf = new ViewPathClusterFinder(oc->numSteps, oc);
    int numFeatures = oc->features->size();
    cout << numFeatures << " features being considered" << endl;
    vector<Feature*>::iterator it;

    if (numFeatures == 2) {
        for (int i = 1; i < 1000; i++) {
            int j = 1000 - i;
            float areaW = ((float) i) / 1000;
            float topoW = ((float) j) / 1000;
            it = oc->features->begin();
            (*it)->setWeight(areaW);
            it++;
            (*it)->setWeight(topoW);

            //find path for this combination
            cout << " finding path where a =" << areaW << " and t= " << topoW << endl;
            findOptimalPath();

            //save path to vpcf
            int* path = new int[oc->numSteps];
            for (int i3 = 0; i3 < oc->numSteps; i3++) {
                path[i3] = oc->path[i3];
            }
            vpcf->addPath(path);

        }
    } else if (numFeatures == 3) {
        int res = 20;
        int numPathsCalculated = 0;
        for (int i = 0; i < res; i++) {

            // for (int j = 1; j < 10; j++) {
            //if (i==0&&j==0) j=1;//quick fix for 0 total weight case.
            //set up path combination
            float areaW = ((float) i) / res;
            float topoW;
            float curvW;

            for (int j = res - i; j > 0; j--) {
                int k = (res - i) - j;
                topoW = ((float) j) / res;
                curvW = ((float) k) / res;
                //                cout<<" ijk "<<i<<" "<<j<<" "<<k<<endl;

                it = oc->features->begin();
                (*it)->setWeight(areaW);
                it++;
                (*it)->setWeight(topoW);
                it++;
                (*it)->setWeight(curvW);

                //find path for this combination
                cout << " finding path where a =" << areaW << " and t= " << topoW << " and c= " << curvW << endl;
                findOptimalPath();

                //save path to vpcf
                int* path = new int[oc->numSteps];
                for (int i3 = 0; i3 < oc->numSteps; i3++) {
                    path[i3] = oc->path[i3];
                }
                vpcf->addPath(path);
                numPathsCalculated++;
            }

        }
        cout << " in total calculated paths : " << numPathsCalculated << endl;
    } else if (numFeatures == 1) {
        for (int j = 1; j < 11; j++) {

            //set up path combination
            float areaW = ((float) j) / 10;
            it = oc->features->begin();
            cout << " wei set to " << areaW << endl;
            (*it)->setWeight(areaW);
            findOptimalPath();
        }
    } else {
        cout << " incorrect number of features for path combinations!! " << endl;
        assert(false);
    }
    meanPaths = vpcf->getPathClusterMeans(numberOfClusters);
    for (int i = 0; i < meanPaths->size(); i++) {
        cout << " path " << i;
        for (int j = 0; j < oc->numSteps; j++) {
            cout << " " << meanPaths->at(i)[j];
        }
        cout << endl;
    }

    vpcf->outputPathsToFile();

}

void Analyser4D::outputPathVis(string filename) {
    oc->pathVisualiser->VisualisePath(oc->path, oc->numSteps);
}

void Analyser4D::outputMeanPaths(string filestem) {
    for (int i = 0; i < meanPaths->size(); i++) {
        cout << "outputting path for cluster " << i << endl;
        stringstream filenameStem;
        filenameStem << filestem << "cluster" << i;
        oc->path = meanPaths->at(i);
        outputPath(filestem);
    }

}

void Analyser4D::outputPath(string filestem) {

    for (int i = 0; i < numSteps; i++) {
        oc->volume4D->setToStep(i);
        oc->volume4D->updateActor();
        string filename(filestem);
        stringstream s;
        s << i;
        filename += s.str();
        filename += ".png";
        cout << " outputting step " << i << " view " << oc->path[i] << endl;
        oc->viewEvaluator->outputView(oc->geoSphere->getView(oc->path[i]), filename.c_str());
    }
}

void Analyser4D::outputBVs(string filestem) {


    for (int i = 0; i < numSteps; i++) {
        oc->volume4D->setToStep(i);
        oc->volume4D->updateActor();
        string filename(filestem);
        stringstream s;
        s << i;
        filename += s.str();
        filename += ".png";
        cout << " outputting BV for  " << i << " view " << oc->path[i] << endl;
        oc->viewEvaluator->outputView(oc->geoSphere->getView(oc->bestViews[i]), filename.c_str());
    }
}

void Analyser4D::outputFeatureScores() {
    vector<Feature*>::iterator it;
    for (it = oc->features->begin(); it != oc->features->end(); it++) {
        (*it)->saveScoreToFile();
    }

}

void Analyser4D::inputFeatureScores() {
    vector<Feature*>::iterator it;
    for (it = oc->features->begin(); it != oc->features->end(); it++) {
        (*it)->loadScoreFromFile();
    }

}

//quick hack to allow me to visually step through the volume so i can see whats going on. used when tweeking isoval to get a good set of results
void Analyser4D::interactSteps() {
  
    for (int step = 0; step < oc->numSteps; step++) {
        cout << "setting to " << step << endl;
        oc->volume4D->setToStep(step);
        //oc->volume4D->findCritcalPoints();
        oc->viewEvaluator->readyFeatures();
        //oc->volume4D->updateActor();
        cout << "set to " << step << endl;
        oc->viewEvaluator->interact();
    }
}

void Analyser4D::vizMeanPaths() {

    oc->pathVisualiser->vizMeanPaths();
}

