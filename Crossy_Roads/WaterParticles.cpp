#include "WaterParticles.h"
#include "Utils.h"
#include "Object.h"
using namespace glm;

void WaterParticleSystem::init(const Assets & assets) {
	mesh = assets.getCubeMesh();
	texture = assets.getTexture("water_plane");
}

void WaterParticleSystem::trigger(vec3 pos) {
	pos.y += mesh->getbbSize().y*0.3f/2;
	this->pos = pos;
	uint nParticles = between(10, 20);
	particles.resize(nParticles);

	for (uint i = 0; i < nParticles; ++i) {
		float horzAngle = between(0.f, 2 * PI);
		float vertAngle = between(0.f, PI / 2);

		particles[i].position = pos;
		particles[i].speed = vec3(cos(horzAngle)*0.2f, sin(vertAngle), sin(horzAngle)*0.2f);
		particles[i].lifetime = (uint)ceil(2 * (-particles[i].speed.y / g));
		particles[i].scale = 0.3f;
		particles[i].state = Falling;
	}
}

void WaterParticleSystem::update() {
	uint j = 0;
	for (uint i = 0; i < particles.size(); i++) {
		particles[i].lifetime -= 1;
		switch (particles[i].state) {
		case Falling:
			particles[i] = particles[j];
			if (particles[j].lifetime > 0) {
				particles[j].speed.y += g;
				particles[j].position += particles[j].speed;
			}
			else {
				particles[j].position.y = pos.y;
				particles[j].state = Stopped;
				particles[j].lifetime = uint(particles[i].scale / 0.01f);
			}
			++j;
			break;
		case Stopped:
			if (particles[i].lifetime > 0) {
				particles[j] = particles[i];
				particles[j].scale += -0.01f;
				++j;
			}
			break;
		}
	}
	particles.resize(j);
}

void WaterParticleSystem::render(ShaderProgram & program) const {
	Object object;
	mesh->setProgramParams(program);
	texture->use();
	for (uint i = 0; i < particles.size(); ++i) {
		object.setPos(particles[i].position);
		object.setScale(vec3(particles[i].scale));
		program.setUniformMatrix4f(modelLoc, *object.getModel());
		mesh->render();
	}
}

WaterParticleSystem::WaterParticleSystem()
{
}


WaterParticleSystem::~WaterParticleSystem()
{
}
