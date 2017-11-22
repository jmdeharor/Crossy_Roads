#version 330

uniform sampler2D tex;
uniform sampler2D shadowMap;

uniform vec4 lightAmbient;
uniform vec4 lightDiffuse;
uniform vec3 lightDirection;

uniform vec4 matAmbient;
uniform vec4 matDiffuse;

in vec4 shadowCoord;
in vec3 pos;
in vec3 N;
in vec2 fragTexCoord;
out vec4 outColor;

void main() {
	vec2 st = shadowCoord.st;
	float trueDepth = shadowCoord.z;
	float storedDepth = texture( shadowMap, st).x;
    float visibility = 1;
	if (!(st.s < 0 || st.s > 1 || st.t < 0 || st.t > 1) && (storedDepth+0.01 < trueDepth)){
		visibility = 0.8;
	}
	vec3 n = normalize(N);
	vec3 texColor = texture(tex, fragTexCoord).xyz;
    outColor = vec4(visibility*texColor,1);//vec4(texColor*(matAmbient*lightAmbient
		//+ matDiffuse*lightDiffuse*max(0.0, dot(n, lightDirection))).xyz, 1);
}

