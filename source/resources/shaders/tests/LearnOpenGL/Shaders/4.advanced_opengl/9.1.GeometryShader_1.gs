//9.GeometryShader_1.gs
#version 330 core

layout(points) in;
//layout(points,  max_vertices = 2) out; // 绘制点
layout(line_strip, max_vertices = 2) out; // 绘制线
 
in VS_OUT {
    vec3 color;
} gs_in[];

out vec3 fColor;

void main()
{

   gl_Position = gl_in[0].gl_Position + vec4(0.1,0.0,0.0,0.0);
   fColor = gs_in[0].color;
   EmitVertex();

   gl_Position = gl_in[0].gl_Position - vec4(0.1,0.0,0.0,0.0);
   EmitVertex();

   EndPrimitive();
}