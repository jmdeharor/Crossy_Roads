#pragma once
#include "Camera\eulerperspcamera.h"
#include "Camera\eulerorthocamera.h"
#include "GameObject.h"

class Camera : public GameObject{
	EulerPerspCamera cam;
	EulerOrthoCamera uiCam;
public:
	void init();
	void update(int deltaTime);
	void render();
	const glm::mat4* getProjectionMatrix() const;
	const glm::mat4* getViewMatrix() const;
	Camera();
	~Camera();
};