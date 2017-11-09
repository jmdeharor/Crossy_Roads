#ifndef OBSCAMERA_H
#define OBSCAMERA_H
#include "basecamera.h"

class OBSCamera : public virtual BaseCamera {
public:
    glm::vec3 OBS, UP;

    OBSCamera(glm::vec3 OBS, glm::vec3 UP);
    virtual ~OBSCamera();
    void updateVM();
};

#endif // OBSCAMERA_H
