#include "eulerorthocamera.h"

EulerOrthoCamera::EulerOrthoCamera(vec3 VRP, float d, float phi,float theta, float psi, float left, float right, float bottom, float top, float zNear, float zFar) :
BaseCamera(VRP, zNear, zFar), EulerCamera(phi, theta, psi, d), OrthoCamera(left, right, top, bottom) {

}

EulerOrthoCamera::~EulerOrthoCamera() {

}
