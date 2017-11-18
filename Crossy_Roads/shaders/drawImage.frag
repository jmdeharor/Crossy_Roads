#version 330 core

// Ouput data
out vec4 outColor;

uniform sampler2D texture;

in vec2 UV;

void main(){
	outColor = texture(texture, UV);
}