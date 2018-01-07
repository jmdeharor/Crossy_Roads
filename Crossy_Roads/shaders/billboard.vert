#version 330

uniform mat4 VP;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out vec2 texCoordFrag;

void main() {
	// Pass texture coordinates to access a given texture atlas
	texCoordFrag = texCoord;
	// Transform position from pixel coordinates to clipping coordinates
	gl_Position = VP * vec4(position, 1.0);
}

