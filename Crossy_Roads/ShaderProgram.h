#ifndef _SHADER_PROGRAM_INCLUDE
#define _SHADER_PROGRAM_INCLUDE


#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include <vector>

typedef unsigned int uint;

// Using the Shader class ShaderProgram can link a vertex and a fragment shader
// together, bind input attributes to their corresponding vertex shader names, 
// and bind the fragment output to a name from the fragment shader

enum UniformLocation {
	modelLoc,
	viewLoc,
	projectionLoc,
	normalMatrixLoc
};

enum VertexAtribLocation {
	positionLoc,
	normalLoc,
	texCoordLoc
};

const std::string uniformOrder[] = { "model", "view", "projection", "normalMatrix" };

class ShaderProgram
{
	std::vector<GLint> uniforms;
public:
	ShaderProgram();

	void init();
	void addShader(const Shader &shader);
	void bindFragmentOutput(const string &outputName);
	GLint bindVertexAttribute(const string &attribName, GLint size, GLsizei stride, GLvoid *firstPointer);
	void bindVertexAttribute(uint location, GLint size, GLsizei stride, GLvoid *firstPointer);
	void link();
	void free();

	void use();

	// Pass uniforms to the associated shaders
	void setUniform2f(const string &uniformName, float v0, float v1);
	void setUniform2f(uint location, float v0, float v1);

	void setUniform3f(const string &uniformName, float v0, float v1, float v2);
	void setUniform3f(uint location, float v0, float v1, float v2);

	void setUniform4f(const string &uniformName, float v0, float v1, float v2, float v3);
	void setUniform4f(uint location, float v0, float v1, float v2, float v3);

	void setUniformMatrix4f(const string &uniformName, const glm::mat4 &mat);
	void setUniformMatrix4f(uint location, const glm::mat4 &mat);

	void setUniformMatrix3f(const string &uniformName, const glm::mat3 &mat);
	void setUniformMatrix3f(uint location, const glm::mat3 &mat);

	void setUniform4f(const string& uniformName, glm::vec4 &vec);
	void setUniform4f(uint location, glm::vec4 &vec);

	uint addUniform(const string& uniformName);

	bool isLinked();
	const string &log() const;

	GLuint getProgramId() const;

private:
	GLuint programId;
	bool linked;
	string errorLog;

};


#endif // _SHADER_PROGRAM_INCLUDE
