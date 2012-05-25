/* 
 * File:   testOVAS.cpp
 * Author: zoizoi
 *
 * Created on 23 January 2011, 14:19
 */

#include <cstdlib>
#include <vtkCellArray.h>
#include <vtkDataSetAttributes.h>
#include "Analyser4D.h"
#include "ImplicitVolume4D.h"
#include "ViewPathClusterFinder.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
   
    Analyser4D* a4d = new Analyser4D;
    a4d->setFile("./ovas.config");
    
    a4d->init();

    a4d->analyse();
   
}
