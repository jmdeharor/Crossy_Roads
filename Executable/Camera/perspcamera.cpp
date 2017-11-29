#include "perspcamera.h"
using namespace glm;

PerspCamera::PerspCamera()
{
}

PerspCamera::PerspCamera(float FOV, float ra) {
    this->FOV = realFOV = FOV;
    this->ra = ra;
}

PerspCamera::~PerspCamera() {

}

void PerspCamera::updatePM() {
    PM = perspective(realFOV, ra, zNear, zFar);
}

void PerspCamera::resize(int h, int w) {
    ra = float(w)/h;
    if (ra < 1) {
        realFOV = 2*(atan(tan(FOV/2)/ra));
    }
}

void PerspCamera::setFOV(float FOV) {
    this->FOV = realFOV = FOV;
}

float PerspCamera::getFOV() const {
    return FOV;
}
