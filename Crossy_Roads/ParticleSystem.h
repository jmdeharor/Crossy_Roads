#ifndef _PARTICLE_SYSTEM_INCLUDE
#define _PARTICLE_SYSTEM_INCLUDE


#include <vector>
#include "Billboard.h"
#include "GameObject.h"

class ParticleSystem : public GameObject {
public:
	struct Particle
	{
		float lifetime;
		glm::vec3 position;
		glm::vec3 speed;
	};

public:
	ParticleSystem();
	~ParticleSystem();

	void firstInit() override;
	void init(const glm::vec2 &billboardQuadSize, const string &billboardTextureName, float gravity = 0.f);

	void trigger(glm::vec3 pos, float radius);
	void update(float deltaTimeInSeconds);
	void render(const glm::vec3 &eye, const glm::mat4& VP);

private:
	ShaderProgram program;
	int numParticles;
	vector<Particle> particles;
	Billboard *billboard;
	float g;
};


#endif // _PARTICLE_SYSTEM_INCLUDE

