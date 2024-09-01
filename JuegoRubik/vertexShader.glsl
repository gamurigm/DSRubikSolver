#version 330 core
layout (location = 0) in vec3 inPos;
//layout (location = 1) in vec3 inFillColor;
layout (location = 1) in vec2 inTexCoords;

//out vec3 outColor;
out vec2 texCoords;

// matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(inPos, 1.0f);
	texCoords = inTexCoords;
}