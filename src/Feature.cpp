/* 
 * File:   Feature.cpp
 * Author: zoizoi
 * 
 * Created on 23 January 2011, 18:28
 */

#include <limits>

#include "Feature.h"


Feature::Feature(){
   
}
Feature::Feature(float weight,OVASControl* o,string name) : weight(weight),oc(o) {
   
    scoreData=ArrayTools::allocate2DArray<float>(oc->numSteps,oc->geoSphere->getNumVs());
    for(int i=0;i<oc->numSteps;i++){
        for(int j=0;j<oc->geoSphere->getNumVs();j++){
            scoreData[i][j]=0;
        }
    }
    actors=new vector<vtkSmartPointer<vtkActor> >();
    
    colourB=1;
    colourG=0;
    colourR=0;
    //cout<<"created feature w "<<weight<<endl;
 //   Feature();
    featureName=name;
    cout << "new feature :" << name<<endl;
}

Feature::Feature(const Feature& orig) {
}

Feature::~Feature() {
}

void Feature::readyRenderer(vtkSmartPointer<vtkRenderer> _renderer) {
    
    //cout<<"ready rend w "<<weight<<endl;
    renderer = _renderer;
    renderWindow = renderer->GetRenderWindow();
    camera = renderer->GetActiveCamera();
   // std::vector< vtkSmartPointer<vtkActor> >::iterator it;
//    for (it = actors.begin(); it != actors.end(); it++) {
//        renderer->AddActor((*it));
//    }
    framebuffer = new FrameBuffer(renderer->GetRenderWindow());
    renderer->AddActor(oc->volActor);
    oc->volActorActive=true;
    oc->volActor->SetVisibility(0);
    
}

void Feature::climbDown() {
    renderer->RemoveActor(oc->volActor);
    oc->volActorActive=false;
    delete framebuffer;
}

void Feature::scoreFeature(GeoPoint* view) {
    
    oc->volActor->SetVisibility(1);
    float viewRange = 3;
    camera->SetPosition(viewRange * view->getx(), viewRange * view->gety(), viewRange * view->getz());
    renderWindow->Render();
   
    
    scoreData[oc->currentStep][oc->currentView]=countColour(framebuffer);
//    cout<<" rawscore feat "<<featureName<<" "<<scoreData[oc->currentStep][oc->currentView]<<endl;
    //cout<<"fscore "<<scoreData[oc->currentStep][oc->currentView]<<endl;
    oc->volActor->SetVisibility(0);                
}

int Feature::countRed(FrameBuffer* fb) {
    fb->grabData();

    int count = 0;
    int len = fb->getLen();
    float* data = fb->getData();
    for (int i = 0; i < len; i++) {
        int j = i * 4;
        if (data[j] > 0.7 && data[j + 1] < 0.3 && data[j + 2] <0.3) {
            count++;
        }
    }
    fb->delData();

    return count;

}

int Feature::countColour(float r, float g, float b, FrameBuffer* fb) {
    fb->grabData();

    int count = 0;
    int len = fb->getLen();
    float* data = fb->getData();
    for (int i = 0; i < len; i++) {
        int j = i * 4;
        if (data[j] == r && data[j + 1] == g && data[j + 2] == b) {
            count++;
        }
    }
    fb->delData();

    return count;

}

float* Feature::getEvaluatedStepData(int step){
    //cout<<"weight this is "<<weight<<endl;
    int zc=0;
    float* data=new float[oc->geoSphere->getNumVs()];
    for(int i=0;i<oc->geoSphere->getNumVs();i++){
        float maxValue = log2(300 * 300);
        float value=scoreData[step][i];
        float contribution;
        contribution = weight * (log2(value) / maxValue);
        //contribution = weight * value;
        if (value == 0){
            contribution = 0;
            zc++;
        }
        data[i]=contribution;
        //cout<<"contrib "<<value<<" "<<weight<<endl;
    }
    //cout<<" zc "<<zc<<endl;
    return data;
}

void Feature::saveScoreToFile(){
    
    
    string filename=string("./lastestTempData/")+featureName+string(".scoreData");
    cout << "outputing file " << filename << "\n";
    ofstream of;
    of.open(filename.c_str());

    of << oc->numSteps << " " << oc->geoSphere->getNumVs() << endl;
    
    for (int i = 0; i < oc->numSteps; i++) {
        for (int j = 0; j < oc->geoSphere->getNumVs(); j++) {
            of << scoreData[i][j] << " ";
            
        }
        of << endl;
    }
    of.close();
    ifstream infile;
}

void Feature::loadScoreFromFile(){
    
    
    string filename=string("./lastestTempData/")+featureName+string(".scoreData");
    cout << "loading file " << filename << "\n";
   
    ifstream inf;
    inf.open(filename.c_str());
    int steps,views;
    inf >> steps >> views;
    cout<<"loading "<<steps<<"steps and "<<views<<" views"<<endl;
    for (int i = 0; i < oc->numSteps; i++) {
        for (int j = 0; j < oc->geoSphere->getNumVs(); j++) {
            inf>>scoreData[i][j] ;
            
        }
        
    }
    inf.close();
    
}
