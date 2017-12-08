#include "Camera.h"
#include "Game.h"
#include <GL\freeglut.h>
#include "Pi.h"
using namespace glm;

void Camera::init(vec3 lightDir, const Player* player) {
	this->player = player;
	cam.VRP = vec3(0);
	cam.d = 20;
	cam.phi = 0;
	cam.psi = 10;
	cam.theta = -5.5f;
	cam.ra = float(CAMERA_WIDTH) / float(CAMERA_HEIGHT);
	cam.setFOV(45.f / 180.f * PI);
	cam.zNear = 0.1f;
	cam.zFar = 100.f;
	cam.updatePM();

	this->lightDir = lightDir*25.f;

	lightCam.OBS = this->lightDir;
	lightCam.VRP = vec3(0);
	lightCam.UP = vec3(0, 1, 0);
	lightCam.left = -30;
	lightCam.right = 11.5f;
	lightCam.bottom = -21.5f;
	lightCam.top = 9;
	lightCam.zNear = 0.01f;
	lightCam.zFar = 100;
	lightCam.updatePM();

	updateVM();
}

void Camera::resize(int w, int h) {
	cam.resize(h, w);
	cam.updatePM();
}

inline void Camera::cameraControl() {
	if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) {
		cam.psi += 0.1f;
		updateVM();
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) {
		cam.psi -= 0.1f;
		updateVM();
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
		cam.d -= 0.1f;
		updateVM();
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
		cam.d += 0.1f;
		updateVM();
	}
	else if (Game::instance().getKey('r')) {
		cam.theta += 0.1f;
		updateVM();
	}
	else if (Game::instance().getKey('f')) {
		cam.theta -= 0.1f;
		updateVM();
	}
}

void Camera::update(int deltaTime) {
	cameraControl();
	vec3 playerPos = player->getPos();
	if (distance(playerPos, cam.VRP) > 0.1f) {
		vec3 diff = playerPos - cam.VRP;
		vec3 absD = abs(diff);
		cam.VRP += diff*0.03f;
		lightCam.VRP = cam.VRP;
		lightCam.OBS = cam.VRP + lightDir;
		updateVM();
	}
}

void Camera::updateVM() {
	cam.updateVM();
	lightCam.updateVM();
	VP = (*cam.getFullProjectionMatrix())*(*cam.getFullViewMatrix());
	VPLight = (*lightCam.getFullProjectionMatrix())*(*lightCam.getFullViewMatrix());
	frustum.setFrustum(&VP[0][0]);
}

const glm::mat4 * Camera::getProjectionMatrix() const {
	return cam.getFullProjectionMatrix();
}

const glm::mat4 * Camera::getViewMatrix() const {
	return cam.getFullViewMatrix();
}

const glm::mat4* Camera::getVPLightMatrix() const {
	return &VPLight;
}

const mat4* Camera::getVPMatrix() const {
	return &VP;
}

glm::vec3 Camera::getPos() const {
	return cam.VRP;
}

void Camera::renderFrustum() const {
	frustum.drawPlanes();
	//frustum.drawNormals();
}

void Camera::setPos(vec3 pos) {
	cam.VRP = pos;
	lightCam.VRP = pos;
	lightCam.OBS = pos + lightDir;
}

const FrustumG & Camera::getFrustum() const {
	return frustum;
}

Camera::Camera()
{
}

Camera::~Camera()
{
}
