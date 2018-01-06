#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 VP;

out vec3 fragNormal;

void main() {
	gl_Position =  VP * model * vec4(vertexPosition_modelspace,1);
	fragNormal = mat3(model)*normal;
}