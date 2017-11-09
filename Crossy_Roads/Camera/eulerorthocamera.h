#ifndef EULERORTHOCAMERA_H
#define EULERORTHOCAMERA_H

#include "eulercamera.h"
#include "orthocamera.h"

class EulerOrthoCamera : public EulerCamera, public OrthoCamera {
public:
    EulerOrthoCamera(vec3 VRP, float d, float phi,float theta, float psi, float left, float right, float bottom, float top, float zNear, float zFar);
    ~EulerOrthoCamera();
};

#endif // EULERORTHOCAMERA_H
