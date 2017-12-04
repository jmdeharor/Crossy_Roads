#include "eulercamera.h"
using namespace glm;

EulerCamera::EulerCamera()
{
}

EulerCamera::EulerCamera(float phi, float theta, float psi, float d) {
    this->phi = phi;
    this->theta = theta;
    this->psi = psi;
    this->d = d;
}

EulerCamera::~EulerCamera() {

}

mat4 EulerCamera::viewMatrixInverse() {
	mat4 inverse = mat4(1);
	inverse = translate(inverse, VRP);
	inverse = rotate(inverse, psi, vec3(0, 1, 0));
	inverse = rotate(inverse, -theta, vec3(1, 0, 0));
	inverse = rotate(inverse, phi, vec3(0, 0, 1));
	inverse = translate(inverse, vec3(0, 0, d));
	return inverse;
}

void EulerCamera::updateVM() {
    VM = translate (mat4(1), vec3(0, 0, -d));
    VM = rotate(VM, -phi, vec3(0, 0, 1));
    VM = rotate(VM, theta, vec3(1, 0, 0));
    VM = rotate(VM, -psi, vec3(0, 1, 0));
    VM = translate(VM, -VRP);
}
