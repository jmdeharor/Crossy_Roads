#version 330

uniform mat4 projection, model, view;
uniform mat3 normalMatrix;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out vec2 texCoordFrag;
out vec3 fragNormal;

void main() {
	fragNormal = normalMatrix*mat3(model)*normal;
	// Pass texture coordinates
	texCoordFrag = texCoord;
	// Transform position from pixel coordinates to clipping coordinates
	gl_Position = projection * view * model * vec4(position, 1.0);
}

