#version 330

uniform mat4 model, VP;

layout (location = 0) in vec3 position;

void main() {
	gl_Position = VP * model * vec4(position, 1.0);
}

