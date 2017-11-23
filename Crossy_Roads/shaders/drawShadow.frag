#version 330 core

uniform sampler2D tex;
uniform sampler2DShadow shadowMap;

uniform vec3 lightDir;

uniform mat4 depthVP, model;
uniform mat4 VP;

in vec4 shadowCoord;
in vec2 fragTexCoord;
in vec3 fragNormal;
out vec4 outColor;

void main(){
	vec2 st = shadowCoord.st;
	float trueDepth = shadowCoord.z;
	float storedDepth = texture(shadowMap, vec3(st, trueDepth-0.0009));
	if (st.s < 0 || st.s > 1 || st.t < 0 || st.t > 1) {
		outColor = vec4(1,0,0,0);
	}
	else outColor = mix(0.5,1,storedDepth)*texture(tex, fragTexCoord);
	//else outColor = vec4(storedDepth);
}