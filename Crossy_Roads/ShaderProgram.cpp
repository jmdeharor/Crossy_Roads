#include <glm/gtc/type_ptr.hpp>
#include "ShaderProgram.h"
using namespace glm;

ShaderProgram::ShaderProgram() {
	uniforms.reserve(sizeof(uniformOrder) / sizeof(string));
	programId = 0;
	linked = false;
}


void ShaderProgram::init()
{
	programId = glCreateProgram();
}

void ShaderProgram::addShader(const Shader &shader)
{
	glAttachShader(programId, shader.getId());
}

void ShaderProgram::bindFragmentOutput(const string &outputName)
{
	glBindAttribLocation(programId, 0, outputName.c_str());
}

GLint ShaderProgram::bindVertexAttribute(const string &attribName, GLint size, GLsizei stride, GLvoid *firstPointer)
{
	GLint attribPos;

	attribPos = glGetAttribLocation(programId, attribName.c_str());
	glVertexAttribPointer(attribPos, size, GL_FLOAT, GL_FALSE, stride, firstPointer);

	return attribPos;
}

void ShaderProgram::bindVertexAttribute(uint location, GLint size, GLsizei stride, GLvoid * firstPointer) {
	glVertexAttribPointer(location, size, GL_FLOAT, GL_FALSE, stride, firstPointer);
}

void ShaderProgram::link()
{
	GLint status;
	char buffer[512];

	glLinkProgram(programId);
	glGetProgramiv(programId, GL_LINK_STATUS, &status);
	linked = (status == GL_TRUE);
	glGetProgramInfoLog(programId, 512, NULL, buffer);
	errorLog.assign(buffer);
}

void ShaderProgram::free()
{
	glDeleteProgram(programId);
}

void ShaderProgram::use()
{
	glUseProgram(programId);
}

bool ShaderProgram::isLinked()
{
	return linked;
}

const string &ShaderProgram::log() const
{
	return errorLog;
}

GLuint ShaderProgram::getProgramId() const {
	return programId;
}

void ShaderProgram::setUniform2f(const string &uniformName, float v0, float v1)
{
	GLint location = glGetUniformLocation(programId, uniformName.c_str());

	if(location != -1)
		glUniform2f(location, v0, v1);
}

void ShaderProgram::setUniform2f(uint location, float v0, float v1) {
	glUniform2f(uniforms[location], v0, v1);
}

void ShaderProgram::setUniform3f(const string &uniformName, float v0, float v1, float v2)
{
	GLint location = glGetUniformLocation(programId, uniformName.c_str());

	if(location != -1)
		glUniform3f(location, v0, v1, v2);
}

void ShaderProgram::setUniform3f(uint location, float v0, float v1, float v2) {
	glUniform3f(uniforms[location], v0, v1, v2);
}

void ShaderProgram::setUniform4f(const string &uniformName, float v0, float v1, float v2, float v3)
{
	GLint location = glGetUniformLocation(programId, uniformName.c_str());

	if(location != -1)
		glUniform4f(location, v0, v1, v2, v3);
}

void ShaderProgram::setUniform4f(uint location, float v0, float v1, float v2, float v3) {
	glUniform4f(uniforms[location], v0, v1, v2, v3);
}

void ShaderProgram::setUniform4f(const string & uniformName, vec4 & vec) {
	GLint location = glGetUniformLocation(programId, uniformName.c_str());

	if (location != -1)
		glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
}

void ShaderProgram::setUniform4f(uint location, vec4 & vec) {
	glUniform4f(uniforms[location], vec.x, vec.y, vec.z, vec.w);
}

uint ShaderProgram::addUniform(const string & uniformName) {
	GLint location = glGetUniformLocation(programId, uniformName.c_str());
	uniforms.push_back(location);
	return uniforms.size() - 1;
}

void ShaderProgram::setUniformMatrix4f(const string &uniformName, const mat4 &mat)
{
	GLint location = glGetUniformLocation(programId, uniformName.c_str());

	if(location != -1)
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

void ShaderProgram::setUniformMatrix4f(uint location, const mat4 & mat) {
	glUniformMatrix4fv(uniforms[location], 1, GL_FALSE, value_ptr(mat));
}

void ShaderProgram::setUniformMatrix3f(const string & uniformName, const mat3 & mat) {
	GLint location = glGetUniformLocation(programId, uniformName.c_str());

	if (location != -1)
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

void ShaderProgram::setUniformMatrix3f(uint location, const mat3 & mat) {
	glUniformMatrix3fv(uniforms[location], 1, GL_FALSE, glm::value_ptr(mat));
}

