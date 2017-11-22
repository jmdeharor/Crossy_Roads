#version 330

uniform sampler2D tex;

uniform vec4 lightAmbient;
uniform vec4 lightDiffuse;
uniform vec3 lightDirection;

uniform vec4 matAmbient;
uniform vec4 matDiffuse;

in vec3 pos;
in vec3 N;
in vec2 fragTexCoord;
out vec4 outColor;

void main() {
	vec3 n = normalize(N);
	vec3 texColor = texture(tex, fragTexCoord).xyz;
    outColor = vec4(texColor*(matAmbient*lightAmbient
		+ matDiffuse*lightDiffuse*max(0.0, dot(n, lightDirection))).xyz, 1);
}