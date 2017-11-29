#ifndef ORTHOCAMERA_H
#define ORTHOCAMERA_H

#include "basecamera.h"

class OrthoCamera : public virtual BaseCamera {
public:
    float left, right, top, bottom, sceneRad;

	OrthoCamera();
    OrthoCamera(float left, float right, float top, float bottom);
	OrthoCamera(float left, float right, float top, float bottom, float sceneRad);
    virtual ~OrthoCamera();
    void updatePM();
    void resize(int h, int w);
};

#endif // ORTHOCAMERA_H
