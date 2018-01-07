#include "ParticleSystem.h"


ParticleSystem::ParticleSystem()
{
	billboard = NULL;
}

ParticleSystem::~ParticleSystem()
{
	if (billboard != NULL)
		delete billboard;
}

void ParticleSystem::init(const glm::vec2 &billboardQuadSize, ShaderProgram &program, const string &billboardTextureName, float gravity)
{
	billboard = Billboard::createBillboard(billboardQuadSize, program, billboardTextureName, BILLBOARD_CENTER);
	g = gravity;
}

void ParticleSystem::addParticle(Particle &newParticle)
{
	particles.push_back(newParticle);
}


void ParticleSystem::update(float deltaTimeInSeconds)
{
	unsigned int j = 0;

	for (unsigned int i = 0; i < particles.size(); i++)
	{
		particles[i].lifetime -= deltaTimeInSeconds;
		if (particles[i].lifetime > 0.f)
		{
			particles[j] = particles[i];
			particles[j].speed -= glm::vec3(0.f, g * deltaTimeInSeconds, 0.f);
			particles[j].position += particles[j].speed * deltaTimeInSeconds;
			j++;
		}
	}
	particles.resize(j);
}

void ParticleSystem::render(const glm::vec3 &eye)
{
	if (billboard == NULL)
		return;
	for (unsigned int i = 0; i < particles.size(); i++)
		billboard->render(particles[i].position, eye);
}


