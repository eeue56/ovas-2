/* 
 * File:   Feature.h
 * Author: zoizoi
 *
 * Created on 23 January 2011, 18:28
 */

#ifndef FEATURE_H
#define	FEATURE_H
#include <vector>

#include "OVASControl.h"
#include "vtkEssentials.h"
#include "FrameBuffer.h"
#include "GeoPoint.h"
#include "GeoSphere.h"
#include "arraytools.h"
//enum FeatureType {Area, Topology, TemporalChange, Curvature};
using namespace std;
class Feature {
public:

    Feature();
    Feature(float weight,OVASControl*,string name);
    Feature(const Feature& orig);
    virtual ~Feature();
    
    virtual void initFeature(){};
//    void addActor(vtkSmartPointer<vtkActor> actor){
//        //actors.push_back(actor);
//    }
    virtual void readyRenderer(vtkSmartPointer<vtkRenderer> _renderer);
    virtual void climbDown();
    virtual void scoreFeature(GeoPoint* view);
    
    int countColour(float r, float g, float b, FrameBuffer* data);
    int countRed( FrameBuffer* data);
    
    int countColour(FrameBuffer* data){
        countColour(colourR,colourG,colourB,data);
    }
    void setWeight(float w){
        weight=w;
    }
    float* getEvaluatedStepData(int step);
    void saveScoreToFile();
    void loadScoreFromFile();
    int a;
    int** intScoreData;
    std::vector< vtkSmartPointer<vtkActor> >* actors;
    
    string featureName;
private:
    
    
    
    
  //  FeatureType type;
    //vtkActor* actors;
    
    
    
protected:
    vtkSmartPointer<vtkCamera> camera;
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkRenderWindow> renderWindow;
    float weight;
    FrameBuffer* framebuffer;
    OVASControl* oc;
    float** scoreData;

    float colourR,colourB,colourG;
    

    

};

#endif	/* FEATURE_H */

