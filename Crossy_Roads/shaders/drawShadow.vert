#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 2) in vec2 texCoord;

uniform mat4 model;
uniform mat4 depthVP;
uniform mat4 VP;

out vec4 shadowCoord;
out vec2 fragTexCoord;

void main(){
	fragTexCoord = texCoord;
	gl_Position =  VP * model * vec4(vertexPosition_modelspace,1);
	shadowCoord =  depthVP * model * vec4(vertexPosition_modelspace,1);
}