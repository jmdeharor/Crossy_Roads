#include "obsperspcamera.h"
using namespace glm;

OBSPerspCamera::OBSPerspCamera()
{
}

OBSPerspCamera::OBSPerspCamera(vec3 VRP, vec3 OBS, vec3 UP, float FOV, float ra, float zNear, float zFar) :
BaseCamera(VRP, zNear, zFar), OBSCamera(OBS, UP), PerspCamera(FOV, ra) {

}
