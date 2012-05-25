/* 
 * File:   ViewEvaluator.cpp
 * Author: zoizoi
 * 
 * Created on 03 February 2011, 22:05
 */

#include "ViewEvaluator.h"

ViewEvaluator::ViewEvaluator() {
}

ViewEvaluator::ViewEvaluator(OVASControl* o) {
    oc = o;
    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderer->SetBackground(0, 0, 0);
    renderWindow->AddRenderer(renderer);
    renderWindow->SetSize(oc->renderXDim, oc->renderYDim);
    camera = vtkSmartPointer<vtkCamera>::New();
    renderer->SetActiveCamera(camera);
    //hack
    renderWindow->SetOffScreenRendering(false);
    renderer->SetBackground(0.04, 0, 0); // Background color dark red

    camera->SetFocalPoint(0.5, 0.5, 0.5);
    camera->SetParallelProjection(1);
    camera->SetClippingRange(0, 8);
    upSet = false;
    upV = new double[3]();
}

ViewEvaluator::ViewEvaluator(const ViewEvaluator& orig) {
}

ViewEvaluator::~ViewEvaluator() {
}

void ViewEvaluator::init() {
    renderer->AddActor(oc->volActor);
}

void ViewEvaluator::readyFeatures() {
    vector<Feature*>::iterator it;
    for (it = oc->features->begin(); it != oc->features->end(); it++) {
      
        Feature* a = *(it);
          cout<<"readying feature "<<a->featureName<<endl;
        (*it)->readyRenderer(renderer);
    }
}

void ViewEvaluator::climbDownFeatures() {
    oc->currentView = 0;
    vector<Feature*>::iterator it;
    for (it = oc->features->begin(); it != oc->features->end(); it++) {
        (*it)->climbDown();
    }
}

void ViewEvaluator::evaluate(GeoPoint* view) {
   
    vector<Feature*>::iterator it;
    
    for (it = oc->features->begin(); it != oc->features->end(); it++) {
        (*it)->scoreFeature(view);
    }


    oc->currentView++;


}

void ViewEvaluator::outputView(GeoPoint* view, string filename) {
    readyFeatures();
    oc->volActor->GetProperty()->SetAmbient(0.3);
    oc->volActor->GetProperty()->SetDiffuse(0.7); //SetShading(0);
    oc->volActor->GetProperty()->SetSpecular(0.7); //SetShading(0);
    oc->volActor->GetProperty()->SetInterpolationToPhong();
oc->volActor->SetVisibility(1);
    vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter = vtkSmartPointer<vtkWindowToImageFilter>::New();
    windowToImageFilter->SetInput(renderWindow);
    vtkSmartPointer<vtkPNGWriter> writer = vtkSmartPointer<vtkPNGWriter>::New();
    writer->SetInput(windowToImageFilter->GetOutput());
    float viewRange = 3;
    vnl_vector<float> pos(3);
    double* upV = new double[3]();
    pos[0] = viewRange * view->getx();
    pos[1] = viewRange * view->gety();
    pos[2] = viewRange * view->getz();
    camera->SetPosition(pos[0], pos[1], pos[2]);

    if (!upSet) {
        upSet = true;
        upV = camera->GetViewUp();
    }
    //upV[0]=0;upV[1]=1;upV[2]=0;
    camera->SetViewUp(upV);
    vnl_vector<float> northPole(3);
    northPole[0] = 0.0001;
    northPole[1] = 1;
    northPole[2] = 0;
    vnl_vector<float> north(3);
    vnl_vector<float> here(3);
    here[0] = pos[0];
    here[1] = pos[1];
    here[2] = pos[2];
    north = northPole - here;
    camera->OrthogonalizeViewUp();
    upV = camera->GetViewUp();
    cout << " up is " << upV[0] << " " << upV[1] << " " << upV[2] << endl;
    cout << " view is " << pos[0] << " " << pos[1] << " " << pos[2] << endl;
    renderWindow->Render();
    windowToImageFilter->Update();
    filename = *(oc->resultsPath) + filename;
    cout << "OUTPUTTING PNG to " << filename << endl;
    writer->SetFileName(filename.c_str());
    writer->Write();


    renderWindow->Render();
    climbDownFeatures();
}

void ViewEvaluator::interact() {
    

    //once only //relocate?
    if (oc->rwiSet == false) {
        oc->rwi =vtkSmartPointer<vtkRenderWindowInteractor>::New();
        oc->rwi->SetRenderWindow(renderWindow);
        oc->rwiSet = true;
    }

    //Add the actors to the scene
    renderer->AddActor(oc->volActor);
    renderer->SetBackground(0, 0, 0); // Background color dark red
    //Render and interact
    renderWindow->Render();
    oc->rwi->Start();
}
