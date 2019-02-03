#version 330 core
layout(location = 0) in vec3 pos; // ����cubemap�����box���㣬��������պͻ���

out vec3 WorldPos; // �����ά����

uniform mat4 projection;
uniform mat4 view;

void main()
{
	WorldPos = pos;
	mat4 rotView = mat4(mat3(view)); // ȥ��ƽ�Ʒ�����ֻʹ����ת�任����
	vec4 clipPos = projection * rotView * vec4(WorldPos, 1.0);
	gl_Position = clipPos.xyww; // zֵ��ԶΪ1����Χ����Զ�����ü���
}