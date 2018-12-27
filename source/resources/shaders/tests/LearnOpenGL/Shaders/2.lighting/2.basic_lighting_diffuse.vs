#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
 

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 normalMat; // ���߾���

out vec3 vFragPos;
out vec3 vNormal;


void main()
{
	// �ڶ�����ɫ���У����ǿ���ʹ��inverse��transpose�����Լ�����������߾���
	// �������������������;�����Ч��ע�����ǻ�Ҫ�ѱ�������ľ���ǿ��ת��Ϊ3��3����
	// ����֤��ʧȥ��λ�������Լ��ܹ�����vec3�ķ�������
	//vNormal = mat3(transpose(inverse(model))) * aNormal; // ��������
	vNormal = mat3(normalMat) * aNormal;
	vFragPos = vec3(model * vec4(aPos, 1.0f));
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
}
