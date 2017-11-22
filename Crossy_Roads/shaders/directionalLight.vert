#version 330

uniform mat4 projection, model, view;
uniform mat3 normalMatrix;
uniform mat4 depthVP;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out vec3 pos;
out vec3 N;
out vec2 fragTexCoord;
out vec4 shadowCoord;

void main() {
	N = mat3(view)*normalMatrix*normal;
	fragTexCoord = texCoord;
	vec4 almostPos = view * model * vec4(position, 1.0);
	pos = almostPos.xyz;
	gl_Position = projection * almostPos;
	shadowCoord =  depthVP * model * vec4(position,1);
}

