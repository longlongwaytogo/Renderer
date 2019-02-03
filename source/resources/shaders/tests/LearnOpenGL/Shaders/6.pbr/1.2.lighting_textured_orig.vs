#version 440 core
//#extension GL_ARB_separate_shader_objects
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

out vec3 WorldPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    TexCoords = texCoords;
    WorldPos = vec3(model * vec4(pos, 1.0f));
    Normal = mat3(model) * normal;   

    gl_Position =  projection * view * vec4(WorldPos, 1.0);
}