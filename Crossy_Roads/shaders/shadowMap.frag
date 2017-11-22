#version 330 core

// Ouput data
out float fragmentdepth;

in vec3 fragNormal;
uniform vec3 lightDir;

void main(){
	fragmentdepth = gl_FragCoord.z;
}