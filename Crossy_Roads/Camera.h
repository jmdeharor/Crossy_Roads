#pragma once
#include "Camera\eulerperspcamera.h"
#include "Camera\eulerorthocamera.h"
#include "UpdatableObject.h"

class Camera : public UpdatableObject {
	EulerPerspCamera cam;
	EulerOrthoCamera uiCam;
public:
	void init();
	void update(int deltaTime);
	const glm::mat4* getProjectionMatrix() const;
	const glm::mat4* getViewMatrix() const;
	Camera();
	~Camera();
};