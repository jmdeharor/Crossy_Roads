#version 330

uniform mat4 projection, modelview;

in vec3 position;
in vec3 color;
out vec3 fragColor;

void main() {
	fragColor = color;
	// Transform position from pixel coordinates to clipping coordinates
	gl_Position = projection * modelview * vec4(position, 1.0);
}

