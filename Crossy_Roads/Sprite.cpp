#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Sprite.h"
using namespace glm;


Sprite *Sprite::createSprite(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram *program)
{
	Sprite *quad = new Sprite(quadSize, sizeInSpritesheet, spritesheet, program);
	return quad;
}

Sprite::Sprite() {
	vao = -1;
}

Sprite::Sprite(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram *program)
{
	init(quadSize, sizeInSpritesheet, spritesheet, program);
}

void Sprite::init(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram *program) {
	float x = quadSize.x / 2;
	float y = quadSize.y / 2;
	float vertices[24] = { -x, -y, 0.f, 0.f,
		x, -y, sizeInSpritesheet.x, 0.f,
		x, y, sizeInSpritesheet.x, sizeInSpritesheet.y,
		-x, -y, 0.f, 0.f,
		x, y, sizeInSpritesheet.x, sizeInSpritesheet.y,
		-x, y, 0.f, sizeInSpritesheet.y };

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertices, GL_STATIC_DRAW);
	posLocation = program->bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = program->bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void *)(2 * sizeof(float)));
	texture = spritesheet;
	shaderProgram = program;
	currentAnimation = -1;
	position = glm::vec2(0);
	rotationAngle = 0;
	animCount = 0;
	size = quadSize;
	animated = true;
	axis = Axis_Z;
}

Sprite::~Sprite() {
	free();
}

void Sprite::update(int deltaTime)
{
	if (animated && currentAnimation >= 0 && animations[currentAnimation].millisecsPerKeyframe > 0)
	{
		timeAnimation += deltaTime;
		while (timeAnimation > animations[currentAnimation].millisecsPerKeyframe)
		{
			timeAnimation -= animations[currentAnimation].millisecsPerKeyframe;
			if (currentKeyframe + 1 == animations[currentAnimation].keyframeDispl.size())
				++animCount;
			currentKeyframe = (currentKeyframe + 1) % animations[currentAnimation].keyframeDispl.size();
		}
		texCoordDispl = animations[currentAnimation].keyframeDispl[currentKeyframe];
	}
}

void Sprite::setSizeSprite(vec2 sizeSprite) {
	size = sizeSprite;
	float x = size.x / 2;
	float y = size.y / 2;
	float vertices[24] = { -x, -y, 0.f, 0.f,
		x, -y, sizeSprite.x, 0.f,
		x, y, sizeSprite.x, sizeSprite.y,
		-x, -y, 0.f, 0.f,
		x, y, sizeSprite.x, sizeSprite.y,
		-x, y, 0.f, sizeSprite.y };
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertices, GL_STATIC_DRAW);
}

void Sprite::render() const {
	glm::mat4 modelview = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 0.f));
	switch (axis) {
	case Axis_X:
		modelview = glm::rotate(modelview, rotationAngle, glm::vec3(1, 0, 0));
		break;
	case Axis_Y:
		modelview = glm::rotate(modelview, rotationAngle, glm::vec3(0, 1, 0));
		break;
	case Axis_Z:
		modelview = glm::rotate(modelview, rotationAngle, glm::vec3(0, 0, 1));
		break;
	}

	shaderProgram->setUniformMatrix4f("modelview", modelview);
	shaderProgram->setUniform2f("texCoordDispl", texCoordDispl.x, texCoordDispl.y);
	glEnable(GL_TEXTURE_2D);
	texture->use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisable(GL_TEXTURE_2D);
}

void Sprite::free() {
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void Sprite::resume() {
	animated = true;
}

void Sprite::pause() {
	animated = false;
}

void Sprite::setNumberAnimations(int nAnimations)
{
	animations.clear();
	animations.resize(nAnimations);
}

void Sprite::setAnimationSpeed(int animId, int keyframesPerSec)
{
	if (animId < int(animations.size()))
		animations[animId].millisecsPerKeyframe = 1000.f / keyframesPerSec;
}

void Sprite::addKeyframe(int animId, const glm::vec2 &displacement)
{
	if (animId < int(animations.size()))
		animations[animId].keyframeDispl.push_back(displacement);
}

void Sprite::changeAnimation(int animId)
{
	if (animId < int(animations.size()))
	{
		animated = true;
		animCount = 0;
		currentAnimation = animId;
		currentKeyframe = 0;
		timeAnimation = 0.f;
		texCoordDispl = animations[animId].keyframeDispl[0];
	}
}

void Sprite::changeAnimationSpecial(int animId, int currentKeyframe) {
	if (animId < int(animations.size()))
	{
		currentAnimation = animId;
		this->currentKeyframe = currentKeyframe;
		texCoordDispl = animations[animId].keyframeDispl[currentKeyframe];
	}
}

int Sprite::getNumKeyframes() const {
	return animations[currentAnimation].keyframeDispl.size();;
}

int Sprite::getCurrentKeyframe() const {
	return currentKeyframe;
}

int Sprite::animation() const
{
	return currentAnimation;
}

void Sprite::move(const glm::vec2 & v) {
	position += v;
}

void Sprite::move(float x, float y) {
	position.x += x;
	position.y += y;
}

void Sprite::setTexture(Texture * spritesheet) {
	texture = spritesheet;
}

void Sprite::setPosition(const glm::vec2 &pos)
{
	position = pos;
}

glm::vec2 Sprite::getPosition() {
	return position;
}

glm::vec2 Sprite::getSize()
{
	return size;
}

bool Sprite::inside(int x, int y) const {
	return x <= position.x+(size.x/2) && x >= position.x-(size.x/2) && y <= position.y+(size.y/2) && y >= position.y-(size.y/2);
}

int Sprite::animRepeats()
{
	return animCount;
}

void Sprite::setRotationAngle(float angle, Axis axis) {
	this->rotationAngle = angle;
	this->axis = axis;
}

float Sprite::getRotationAngle() {
	return rotationAngle;
}

ShaderProgram * Sprite::getShaderProgram()
{
	return shaderProgram;
}