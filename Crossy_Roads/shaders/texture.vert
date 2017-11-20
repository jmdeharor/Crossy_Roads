#version 330

uniform mat4 projection, model, view;
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out vec2 texCoordFrag;

void main()
{
	// Pass texture coordinates
	texCoordFrag = texCoord;
	// Transform position from pixel coordinates to clipping coordinates
	gl_Position = projection * view * model * vec4(position, 1.0);
}

