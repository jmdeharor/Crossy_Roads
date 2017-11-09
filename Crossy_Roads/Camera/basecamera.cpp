#include "basecamera.h"
using namespace glm;

BaseCamera::BaseCamera() {
}

BaseCamera::BaseCamera(vec3 VRP, float zNear, float zFar) {
    this->VRP = VRP;
    this->zNear = zNear;
    this->zFar = zFar;
}

BaseCamera::~BaseCamera() {

}

const float* BaseCamera::getViewMatrix() const {
    return &VM[0][0];
}

const float* BaseCamera::getProjectionMatrix() const {
    return &PM[0][0];
}

const mat4* BaseCamera::getFullViewMatrix() const {
    return &VM;
}
