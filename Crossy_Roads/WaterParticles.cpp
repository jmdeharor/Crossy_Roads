#include "WaterParticles.h"
#include "Utils.h"
#include "Object.h"
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


void WaterParticleSystem::firstInit() {
	compileShader(program, "simpleColor");
	program.bindFragmentOutput("outColor");

	colorLoc = program.addUniform("color");
	lightLoc = program.addUniform("lightDir");
}

void WaterParticleSystem::init(const Assets & assets) {
	GameObject::init();
	mesh = assets.getCubeMesh();
	texture = assets.getTexture("water_plane");
}

void WaterParticleSystem::trigger(vec3 pos, uint numParticles, vec4 color) {
	this->color = color;
	pos.y += mesh->getbbSize().y*0.3f/2;
	this->pos = pos;
	uint nParticles = numParticles + between(-5, 5);
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

void WaterParticleSystem::render(const mat4& VP, vec3 lightDir) {
	if (particles.size() == 0)
		return;
	program.use();
	program.setUniformMatrix4f(viewProjectionLoc, VP);
	program.setUniform4f(colorLoc, color);
	program.setUniform3f(lightLoc, lightDir);
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
