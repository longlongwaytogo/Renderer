
#version 330 core
layout(location = 0) in vec3 pos;
layout(location = 2) in vec2 texCoords;
layout(location = 3) in mat4 instanceModel;
 

uniform mat4 view;
uniform mat4 projection;
 
out vec2 TexCoords;
void main()
{
	TexCoords = texCoords;
	gl_Position = projection * view * instanceModel * vec4(pos,1.0f);
}
