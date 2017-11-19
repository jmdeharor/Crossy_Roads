#pragma once
#include "Camera\eulerperspcamera.h"
#include "Camera\eulerorthocamera.h"

class Camera {
	EulerPerspCamera cam;
	EulerOrthoCamera uiCam;
public:
	void init();
	void resize(int w, int h);
	void update(int deltaTime);
	void updateVM();
	const glm::mat4* getProjectionMatrix() const;
	const glm::mat4* getViewMatrix() const;
	void setPos(glm::vec3 pos);
	Camera();
	~Camera();
};