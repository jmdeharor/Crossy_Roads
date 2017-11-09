#ifndef OBSPERSPCAMERA_H
#define OBSPERSPCAMERA_H
#include "obscamera.h"
#include "perspcamera.h"

class OBSPerspCamera : public OBSCamera, public PerspCamera {
public:
    OBSPerspCamera(glm::vec3 VRP, glm::vec3 OBS, glm::vec3 UP, float FOV, float ra, float zNear, float zFar);
};

#endif // OBSPERSPCAMERA_H
