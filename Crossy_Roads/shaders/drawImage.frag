#version 330 core

// Ouput data
out vec4 outColor;

uniform sampler2D tex;

in vec2 UV;

void main(){
	outColor = vec4(texture(tex, UV).z);
}