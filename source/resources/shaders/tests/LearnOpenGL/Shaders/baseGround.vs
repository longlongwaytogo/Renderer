// BaseGround.vs
#version 330 core

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vTexcoord;

uniform mat4 projection;
uniform mat4 view;

out VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec2 Texcoords;
} vs_out;

void main()
{
	vs_out.FragPos = vPos;
	vs_out.Normal = vNormal;
	vs_out.Texcoords = vTexcoord;
	gl_Position =  projection * view * vec4(vPos,1.0);
}