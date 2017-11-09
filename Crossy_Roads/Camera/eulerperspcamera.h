#ifndef EULERPERSPCAMERA_H
#define EULERPERSPCAMERA_H
#include "eulercamera.h"
#include "perspcamera.h"

class EulerPerspCamera : public EulerCamera, public PerspCamera {
public:
    EulerPerspCamera(glm::vec3 VRP, float d, float phi, float theta, float psi, float FOV, float ra, float zNear, float zFar);
    ~EulerPerspCamera();
};

#endif // EULERPERSPCAMERA_H
