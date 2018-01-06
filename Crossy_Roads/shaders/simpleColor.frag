#version 330 core

uniform vec4 color;

uniform vec3 lightDir;

in vec3 fragNormal;

out vec4 outColor;

void main() {
	vec3 N = normalize(fragNormal);
	float diffuse = max(0, dot(N,lightDir));
	outColor = mix(0.5, 1, diffuse)*color;
}