#ifndef ORTHOCAMERA_H
#define ORTHOCAMERA_H

#include "basecamera.h"

class OrthoCamera : public virtual BaseCamera {
public:
    float left, right, top, bottom;

    OrthoCamera(float left, float right, float top, float bottom);
    virtual ~OrthoCamera();
    void updatePM();
    void resize(int h, int w, float sceneRad);
};

#endif // ORTHOCAMERA_H
