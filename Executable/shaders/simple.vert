#version 330

uniform mat4 projection, model, view;
uniform mat3 normalMatrix;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

void main() {
	gl_Position = projection * view * model * vec4(position, 1.0);
}

