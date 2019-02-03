#version 330 core
layout(location = 0) in vec3 pos; // 绘制cubemap输入的box顶点，类似于天空和绘制

out vec3 WorldPos; // 输出三维坐标

uniform mat4 projection;
uniform mat4 view;

void main()
{
	WorldPos = pos;
	mat4 rotView = mat4(mat3(view)); // 去掉平移分量，只使用旋转变换矩阵
	vec4 clipPos = projection * rotView * vec4(WorldPos, 1.0);
	gl_Position = clipPos.xyww; // z值永远为1，包围盒永远不被裁剪掉
}