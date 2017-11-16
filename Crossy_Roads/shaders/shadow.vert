#version 330

uniform mat4 projection, model, view;

in vec3 position;

void main() {
	gl_Position = projection * view * model * vec4(position, 1.0);
}

