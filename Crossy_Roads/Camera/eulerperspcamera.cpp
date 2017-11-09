#include "eulerperspcamera.h"
using namespace glm;

EulerPerspCamera::EulerPerspCamera(vec3 VRP, float d, float phi, float theta, float psi, float FOV, float ra, float zNear, float zFar) :
BaseCamera(VRP, zNear, zFar), EulerCamera(phi, theta, psi, d), PerspCamera(FOV, ra) {
}

EulerPerspCamera::~EulerPerspCamera() {

}
