#pragma once
#include "Assets.h"
#include "ShaderProgram.h"
#include <glm\glm.hpp>
#include <vector>

class WaterParticleSystem {
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
public:
	float g;
public:
	void init(const Assets& assets);
	void trigger(glm::vec3 pos);
	void update();
	void render(ShaderProgram& program) const;
	WaterParticleSystem();
	~WaterParticleSystem();
};

