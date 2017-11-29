#include "orthocamera.h"
using namespace glm;

OrthoCamera::OrthoCamera()
{
}

OrthoCamera::OrthoCamera(float left, float right, float top, float bottom) {
    this->left = left;
    this->right = right;
    this->top = top;
    this->bottom = bottom;
}

OrthoCamera::OrthoCamera(float left, float right, float top, float bottom, float sceneRad) {
	this->left = left;
	this->right = right;
	this->top = top;
	this->bottom = bottom;
	this->sceneRad = sceneRad;
}

OrthoCamera::~OrthoCamera() {

}

void OrthoCamera::updatePM() {
    PM = ortho(left, right, bottom, top, zNear, zFar);
}

void OrthoCamera::resize(int h, int w) {
    float ra = float(w)/h;
    if (ra >= 1) {
        left = -sceneRad*ra;
        right = sceneRad*ra;
        top = sceneRad;
        bottom = -sceneRad;
    }
    else {
        left = -sceneRad;
        right = sceneRad;
        top = sceneRad/ra;
        bottom = -sceneRad/ra;
    }
}
