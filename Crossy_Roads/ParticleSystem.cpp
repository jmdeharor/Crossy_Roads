#include "ParticleSystem.h"
#include <iostream>
#include "Utils.h"
using namespace std;
using namespace glm;

inline void compileShader(ShaderProgram& program, const string& fileName) {
	Shader vShader, fShader;
	string path = "shaders/" + fileName;
	vShader.initFromFile(VERTEX_SHADER, path + ".vert");
	if (!vShader.isCompiled()) {
		cout << "Vertex Shader " + fileName + " Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, path + ".frag");
	if (!fShader.isCompiled()) {
		cout << "Fragment Shader " + fileName + " Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	program.init();
	program.addShader(vShader);
	program.addShader(fShader);
	program.link();
	if (!program.isLinked()) {
		cout << "Shader " + fileName + " Linking Error" << endl;
		cout << "" << program.log() << endl << endl;
	}
	vShader.free();
	fShader.free();
	for (uint i = 0; i < sizeof(uniformOrder) / sizeof(string); ++i) {
		program.addUniform(uniformOrder[i]);
	}
}


ParticleSystem::ParticleSystem()
{
	billboard = NULL;
}

ParticleSystem::~ParticleSystem()
{
	if (billboard != NULL)
		delete billboard;
}

void ParticleSystem::firstInit() {
	compileShader(program, "billboard");
	program.bindFragmentOutput("outColor");
}

void ParticleSystem::init(const glm::vec2 &billboardQuadSize, const string &billboardTextureName, float gravity) {
	GameObject::init();
	billboard = Billboard::createBillboard(billboardQuadSize, program, billboardTextureName, BILLBOARD_CENTER);
	g = gravity;
}

void ParticleSystem::trigger(vec3 pos, float radius) {
	uint nParticles = between(20, 30);

	for (uint i = 0; i < nParticles; ++i) {
		Particle particle;
		particle.lifetime = 60;
		particle.position = pos + vec3(between(-radius, radius), between(-radius, radius), between(-radius, radius));
		particle.speed = vec3(0, between(0.2f, 0.3f), 0);
		particles.push_back(particle);
	}
}

void ParticleSystem::update(float deltaTimeInSeconds)
{
	unsigned int j = 0;

	for (unsigned int i = 0; i < particles.size(); i++)
	{
		particles[i].lifetime -= 1;
		if (particles[i].lifetime > 0.f)
		{
			particles[j] = particles[i];
			particles[j].speed -= glm::vec3(0.f, g, 0.f);
			particles[j].position += particles[j].speed;
			j++;
		}
	}
	particles.resize(j);
}

void ParticleSystem::render(const vec3 &eye, const mat4& VP) {
	if (particles.size() == 0)
		return;
	glEnable(GL_BLEND);
	program.use();
	program.setUniformMatrix4f(viewProjectionLoc, VP);
	for (unsigned int i = 0; i < particles.size(); i++)
		billboard->render(particles[i].position, eye);
	glDisable(GL_BLEND);
}


