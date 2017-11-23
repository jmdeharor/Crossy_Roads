#pragma once
#include "Camera\eulerperspcamera.h"
#include "Camera\obsorthocamera.h"

class Camera {
	glm::vec3 lightDir;
	EulerPerspCamera cam;
	OBSOrthoCamera uiCam, lightCam;
	bool cameraMode;
public:
	void init(glm::vec3 lightDir);
	void resize(int w, int h);
	void update(int deltaTime);
	void updateVM();
	const glm::mat4* getProjectionMatrix() const;
	const glm::mat4* getViewMatrix() const;
	glm::mat4 getVPLightMatrix() const;
	glm::mat4 getVPMatrix() const;
	void setPos(glm::vec3 pos);
	Camera();
	~Camera();
};