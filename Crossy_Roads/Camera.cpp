#include "Camera.h"
#include "Game.h"
#include <GL\freeglut.h>
#define PI 3.141592654f

void Camera::init() {
	cam.d = 1;
	cam.phi = 0;
	cam.psi = 0;
	cam.theta = 0;
	cam.ra = float(CAMERA_WIDTH) / float(CAMERA_HEIGHT);
	cam.setFOV(45.f / 180.f * PI);
	cam.zNear = 0.1f;
	cam.zFar = 100.f;
	cam.updatePM();
	cam.updateVM();
}

void Camera::resize(int w, int h) {
	cam.resize(h, w);
	cam.updatePM();
}

void Camera::update(int deltaTime) {
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
	else if (Game::instance().getKey('w')) {
		cam.theta += 0.1f;
		cam.updateVM();
	}
	else if (Game::instance().getKey('s')) {
		cam.theta -= 0.1f;
		cam.updateVM();
	}
	else if (Game::instance().getKey('q')) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else if (Game::instance().getKey('e')) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
}

const glm::mat4 * Camera::getProjectionMatrix() const
{
	return cam.getFullProjectionMatrix();
}

const glm::mat4 * Camera::getViewMatrix() const
{
	return cam.getFullViewMatrix();
}

Camera::Camera()
{
}

Camera::~Camera()
{
}
