//9.ssao.vs
#version 330 core

layout(location = 0) out vec4 gPositionDepth;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec4 gAlbedoSpec;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

const float NEAR = 0.1f; 
const float FAR = 50.0f;

float LinearizeDepth(float depth)
{
	float z = depth * 2.0 - 1.0; // to NDC 
	return (2.0 * NEAR * FAR) / (FAR + NEAR - z * (FAR - NEAR));
}

void main()
{
	// 存储片元的第一个位置到G缓冲纹理
	gPositionDepth.xyz = FragPos;
	// 存储现行深度
	gPositionDepth.a = LinearizeDepth(gl_FragCoord.z);
	// 储存法线信息到G缓冲
	gNormal = normalize(Normal);
	// 和漫反射颜色
	gAlbedoSpec.rgb = vec3(0.95);
}
