//9.GeometryShader_1.gs
#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;
 
in vec2 texCoords[];
in vec3 Normal[];

const float MAGNITUDE = 0.4;

void GenerateLine(int index)
{
	gl_Position = gl_in[index].gl_Position;
	EmitVertex();
	gl_Position = gl_in[index].gl_Position + vec4(Normal[index],0.0) * MAGNITUDE;
	EmitVertex();
	EndPrimitive();
}
 
void main()
{
	GenerateLine(0);
	GenerateLine(1);
	GenerateLine(2);
}