#include "Camera.h"
#include "Game.h"
#include <GL\freeglut.h>
#include "Pi.h"
using namespace glm;

void Camera::init(glm::vec3 lightDir) {
	cam.d = 20;
	cam.phi = 0;
	cam.psi = 10;
	cam.theta = -5.5f;
	cam.ra = float(CAMERA_WIDTH) / float(CAMERA_HEIGHT);
	cam.setFOV(45.f / 180.f * PI);
	cam.zNear = 0.1f;
	cam.zFar = 100.f;
	cam.updatePM();
	cam.updateVM();

	lightCam.OBS = lightDir*25.f;
	lightCam.VRP = vec3(0);
	lightCam.UP = vec3(0, 1, 0);
	lightCam.left = -28;
	lightCam.right = 11.5f;
	lightCam.bottom = -25;
	lightCam.top = 25;
	lightCam.zNear = 0.01f;
	lightCam.zFar = 100;
	lightCam.updatePM();
	lightCam.updateVM();
	cameraMode = true;
}

void Camera::resize(int w, int h) {
	cam.resize(h, w);
	cam.updatePM();
}

void Camera::update(int deltaTime) {
	if (Game::instance().getKey('l')) {
		cameraMode = false;
	}
	else if (Game::instance().getKey('k')) {
		cameraMode = true;
	}
	if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) {
		cam.psi += 0.1f;
		cam.updateVM();
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) {
		cam.psi -= 0.1f;
		cam.updateVM();
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
		cam.d -= 0.1f;
		cam.updateVM();
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
		cam.d += 0.1f;
		cam.updateVM();
	}
	else if (Game::instance().getKey('r')) {
		cam.theta += 0.1f;
		cam.updateVM();
	}
	else if (Game::instance().getKey('f')) {
		cam.theta -= 0.1f;
		cam.updateVM();
	}
}

void Camera::updateVM() {
	cam.updateVM();
}

const glm::mat4 * Camera::getProjectionMatrix() const
{
	return cam.getFullProjectionMatrix();
}

const glm::mat4 * Camera::getViewMatrix() const
{
	return cam.getFullViewMatrix();
}

mat4 Camera::getVPLightMatrix() const {
	return (*lightCam.getFullProjectionMatrix())*(*lightCam.getFullViewMatrix());
}

mat4 Camera::getVPMatrix() const {
	return cameraMode ? (*cam.getFullProjectionMatrix())*(*cam.getFullViewMatrix()) : (*lightCam.getFullProjectionMatrix())*(*lightCam.getFullViewMatrix());
}

void Camera::setPos(glm::vec3 pos) {
	cam.VRP = pos;
}

Camera::Camera()
{
}

Camera::~Camera()
{
}
