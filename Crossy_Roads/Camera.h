#pragma once
#include "Camera\eulerperspcamera.h"
#include "Camera\obsorthocamera.h"
#include "Player.h"
#include "FrustumG.h"

class Camera {
private:
	const Player* player;
	glm::vec3 lightDir;
	EulerPerspCamera cam;
	glm::mat4 VP;
	glm::mat4 VPLight;
	OBSOrthoCamera uiCam, lightCam;
	FrustumG frustum;
private:
	void cameraControl();
public:
	void init(glm::vec3 lightDir, const Player* player);
	void resize(int w, int h);
	void update(int deltaTime);
	void updateVM();
	const glm::mat4* getProjectionMatrix() const;
	const glm::mat4* getViewMatrix() const;
	const glm::mat4* getVPLightMatrix() const;
	const glm::mat4* getVPMatrix() const;
	glm::vec3 getPos() const;
	void renderFrustum() const;
	void setPos(glm::vec3 pos);
	const FrustumG& getFrustum() const;
	Camera();
	~Camera();
};