/* 
 * File:   FrameBuffer.cpp
 * Author: zoizoi
 * 
 * Created on 30 January 2011, 20:29
 */

#include <vtkWindow.h>

#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(vtkSmartPointer<vtkRenderWindow> _renderWindow) {
    renderWindow=_renderWindow;
}

FrameBuffer::FrameBuffer(const FrameBuffer& orig) {
}


