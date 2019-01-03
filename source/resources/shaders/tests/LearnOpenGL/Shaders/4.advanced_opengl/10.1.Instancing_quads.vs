//10.1.Instancing_quads.vs
#version 330 core
layout(location = 0) in vec2 pos;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 offset;

out vec3 vs_color;
uniform vec2 offsets[100];
uniform bool use_instance_array;

void main()
{
		if(use_instance_array)
			gl_Position = vec4( pos * (gl_InstanceID / 100.0f) + offset,0.0f,1.0f);
		else
			gl_Position = vec4(pos + offsets[gl_InstanceID],0.0f,1.0f);

		vs_color = color;
}
