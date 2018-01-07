#pragma once
#include "Assets.h"
#include "ShaderProgram.h"
#include <glm\glm.hpp>
#include <vector>
#include "GameObject.h"

class WaterParticleSystem : public GameObject {
	enum State {
		Falling,
		Stopped
	};
	struct Particle {
		glm::uint lifetime;
		glm::vec3 position;
		float scale;
		glm::vec3 speed;
		State state;
	};
	std::vector<Particle> particles;
	const Mesh* mesh;
	const Texture* texture;
	glm::vec3 pos;
	ShaderProgram program;
	glm::uint colorLoc;
	glm::uint lightLoc;
	glm::vec4 color;
public:
	float g;
public:
	void firstInit() override;
	void init(const Assets& assets);
	void trigger(glm::vec3 pos, glm::uint numParticles, glm::vec4 color);
	void update();
	void render(const glm::mat4& VP, glm::vec3 lightDir);
	WaterParticleSystem();
	~WaterParticleSystem();
};

