//9.2.GeometryShader.vs
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec2 texCoords;
out vec3 Normal;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	mat3 normalMatrix = mat3(transpose(inverse(view*model)));
	Normal = normalize(vec3(projection * vec4(normalMatrix * aNormal,0.0)));
	texCoords = aTexCoords;
	gl_Position = projection * view * model * vec4(aPos,1.0);
}
