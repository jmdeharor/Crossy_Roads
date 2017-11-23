#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 normal;

uniform vec3 lightDir;

// Values that stay constant for the whole mesh.
uniform mat4 depthVP, model;

out vec3 fragNormal;

void main(){
	fragNormal = normal;
	gl_Position =  depthVP * model * vec4(vertexPosition_modelspace,1);
}