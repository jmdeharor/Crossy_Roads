#version 330

uniform sampler2D tex;

uniform vec4 lightAmbient;
uniform vec4 lightDiffuse;
uniform vec4 lightPosition;

uniform vec4 matAmbient;
uniform vec4 matDiffuse;

in vec3 pos;
in vec3 N;
in vec2 fragTexCoord;
out vec4 outColor;

void main() {
	vec3 n = normalize(N);
    vec3 L = normalize(lightPosition.xyz - pos);
	vec4 texColor = texture(tex, fragTexCoord);
    outColor = texColor*(matAmbient*lightAmbient
		+ matDiffuse*lightDiffuse*max(0.0, dot(n, L)));
}

