#include "obscamera.h"
using namespace glm;

OBSCamera::OBSCamera()
{
}

OBSCamera::OBSCamera(vec3 OBS, vec3 UP) {
    this->OBS = OBS;
    this->UP = UP;
}

OBSCamera::~OBSCamera() {

}

void OBSCamera::updateVM() {
    VM = lookAt(OBS, VRP, UP);
}
