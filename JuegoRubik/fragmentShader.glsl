#version 330 core
out vec4 FragColor;
in vec2 texCoords;

uniform bool fill;
uniform sampler2D texture0;
uniform sampler2D texture1;

uniform vec3 changingColor;
uniform vec3 lightColor;
uniform float mixRatio;

void main()
{
	if(fill) {
		FragColor =  vec4(changingColor, 1.0);
	}
	else {
		FragColor = vec4(changingColor, 1.0);
	}
}