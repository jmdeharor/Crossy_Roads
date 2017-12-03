#version 330 core

// Ouput data
out float fragmentdepth;

void main(){
	fragmentdepth = gl_FragCoord.z;
}