#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;

uniform mat4 model;
uniform mat4 depthVP;
uniform mat4 VP;

out vec4 shadowCoord;

void main(){
	gl_Position =  VP * model * vec4(vertexPosition_modelspace,1);
	shadowCoord =  depthVP * model * vec4(vertexPosition_modelspace,1);
}