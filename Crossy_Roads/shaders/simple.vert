#version 330

uniform mat4 projection, modelview;
uniform mat3 normalMatrix;

in vec3 position;
in vec3 normal;
in vec3 color;
out vec3 fragColor;

void main() {
	vec3 N = normalize(normalMatrix*normal);
	fragColor = color * N.z;
	// Transform position from pixel coordinates to clipping coordinates
	gl_Position = projection * modelview * vec4(position, 1.0);
}

