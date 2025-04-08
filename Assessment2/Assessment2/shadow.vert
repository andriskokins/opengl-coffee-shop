#version 450 core

layout (location = 0) in vec4 vPos;

uniform mat4 LightSpaceMatrix;
uniform mat4 model;

void main()
{
	gl_Position = LightSpaceMatrix * model * vPos;
}