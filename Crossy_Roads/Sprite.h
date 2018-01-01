#ifndef _SPRITE_INCLUDE
#define _SPRITE_INCLUDE


#include <vector>
#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
#include "AnimKeyframes.h"


// This class is derived from code seen earlier in TexturedQuad but it is also
// able to manage animations stored as a spritesheet. 

enum Axis {
	Axis_X,
	Axis_Y,
	Axis_Z
};

class Sprite
{

public:
	// Textured quads can only be created inside an OpenGL context
	static Sprite *createSprite(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram *program);

	Sprite();
	Sprite(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram *program);
	void init(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram *program);
	~Sprite();
	void update(int deltaTime);
	void render() const;
	void free();

	void resume();
	void pause();
	void setNumberAnimations(int nAnimations);
	void setAnimationSpeed(int animId, int keyframesPerSec);
	void addKeyframe(int animId, const glm::vec2 &frame);
	void changeAnimation(int animId);
	void changeAnimationSpecial(int animId, int currentKeyframe);
	int getNumKeyframes() const;
	int getCurrentKeyframe() const;
	int animation() const;
	void move(const glm::vec2 &v);
	void move(float x, float y);
	void setTexture(Texture* spritesheet);
	void setSizeSprite(glm::vec2 sizeSprite);
	void setPosition(const glm::vec2 &pos);
	glm::vec2 getPosition();
	glm::vec2 getSize();
	int animRepeats();
	void setRotationAngle(float angle, Axis axis);
	float getRotationAngle();
	ShaderProgram* getShaderProgram();

private:
	Axis axis;
	bool animated;
	int animCount;
	Texture *texture;
	ShaderProgram *shaderProgram;
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	glm::vec2 position;
	int currentAnimation, currentKeyframe;
	float timeAnimation;
	glm::vec2 texCoordDispl;
	glm::vec2 size;
	vector<AnimKeyframes> animations;
	float rotationAngle;
};


#endif // _SPRITE_INCLUDE
