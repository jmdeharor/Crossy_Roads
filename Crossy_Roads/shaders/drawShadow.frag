#version 330 core

uniform sampler2D shadowMap;
uniform mat4 depthVP, model;
uniform mat4 VP, bias;

in vec4 shadowCoord;

// Ouput data
out vec4 outColor;

void main(){
	vec4 realShadowCoord = bias*depthVP*model*shadowCoord;
	vec2 st = realShadowCoord.st;
	float trueDepth = realShadowCoord.x;
	float storedDepth = texture( shadowMap, st).x;
    float visibility = 1.0;
	vec4 texColor = texture( shadowMap, st);
	if ( texture( shadowMap, st).x <= trueDepth){
		visibility = 0;
	}
	outColor = vec4(storedDepth);
}