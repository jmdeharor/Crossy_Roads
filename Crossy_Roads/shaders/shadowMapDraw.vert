#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

uniform mat4 model;
uniform mat4 depthVP;
uniform mat4 VP;
uniform mat3 normalMatrix;

out vec4 shadowCoord;
out vec2 fragTexCoord;
out vec3 fragNormal;

void main() {
	fragTexCoord = texCoord;
	gl_Position =  VP * model * vec4(vertexPosition_modelspace,1);
	shadowCoord =  depthVP * model * vec4(vertexPosition_modelspace,1);
	fragNormal = mat3(model)*normal;
}