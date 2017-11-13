#ifndef PERSPCAMERA_H
#define PERSPCAMERA_H
#include "basecamera.h"

class PerspCamera : public virtual BaseCamera {
    float FOV, realFOV;
public:
    float ra;

	PerspCamera();
    PerspCamera(float FOV, float ra);
    virtual ~PerspCamera();
    void updatePM();
    void resize(int h, int w);
    void setFOV(float FOV);
    float getFOV() const;
};

#endif // PERSPCAMERA_H
