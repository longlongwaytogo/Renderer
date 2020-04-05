//3.1.1.shadow_mapping_depth.vs
#version 330 core

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vTexcoord;


uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out VS_OUT{
	vec3 FragPos;
	vec3 Normal;
	vec2 Texcoords;
} vs_out;

//out vec2 TexCoords;
void main()
{
	vs_out.FragPos = vPos;
	vs_out.Normal = vNormal;
	vs_out.Texcoords = vTexcoord;
	 
	gl_Position = projection * view * model * vec4(vPos, 1.0f);
}