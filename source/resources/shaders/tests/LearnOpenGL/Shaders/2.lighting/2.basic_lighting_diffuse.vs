#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
 

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 normalMat; // 法线矩阵

out vec3 vFragPos;
out vec3 vNormal;


void main()
{
	// 在顶点着色器中，我们可以使用inverse和transpose函数自己生成这个法线矩阵，
	// 这两个函数对所有类型矩阵都有效。注意我们还要把被处理过的矩阵强制转换为3×3矩阵，
	// 来保证它失去了位移属性以及能够乘以vec3的法向量。
	//vNormal = mat3(transpose(inverse(model))) * aNormal; // 修正法线
	vNormal = mat3(normalMat) * aNormal;
	vFragPos = vec3(model * vec4(aPos, 1.0f));
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
}
