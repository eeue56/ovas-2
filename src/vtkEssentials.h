/* 
 * File:   vtkEssentials.h
 * Author: zoizoi
 *
 * Created on 23 January 2011, 18:20
 */

#ifndef VTKESSENTIALS_H
#define	VTKESSENTIALS_H
#include <vtkImageData.h>
#include <vtkRenderer.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkCamera.h>
#include <vtkSmartPointer.h>
#include <vtkContourFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkConeSource.h>
#include <vtkPolyData.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkProperty.h>
#include <vtkWindowToImageFilter.h>
#include <vtkPNGWriter.h>
#include <vtkSphereSource.h>
#include "vtkLineSource.h"
#include <vtkThreshold.h>
#include <vtkReebGraph.h>
#include <vtkUnstructuredGrid.h>
#include <vtkUnstructuredGridToReebGraphFilter.h>
#include <vtkGeometryFilter.h>
#include <vtkAppendFilter.h>
#include <vtkPointData.h>
#include <vtkDataSetTriangleFilter.h>
#include <vtkReebGraphSimplificationFilter.h>
#include <vtkTable.h>
#include <vtkDoubleArray.h>
#include <vtkVariant.h>
#include <vtkVariantArray.h>
#include <vtkEdgeListIterator.h>
#include <vtkReebGraphVolumeSkeletonFilter.h>
#include "vtkTransform.h"
#include <vtkTriangle.h>
#include <vtkCellArray.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include "vtkLookupTable.h"
#include "vtkXMLPolyDataWriter.h"
#include "vtkFloatArray.h"
#include <vtkDelaunay2D.h>
#include <vtkMath.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkXMLPolyDataWriter.h>
#include "vtkMath.h"

#define vtkNew(type,name) \
       vtkSmartPointer<type> name = vtkSmartPointer<type>::New()
#endif	/* VTKESSENTIALS_H */

