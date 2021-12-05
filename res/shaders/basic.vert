#version 330 core
layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec2 a_Tex;

out vec2 v_Texture;

void main()
{
    gl_Position = vec4(a_Pos, 1.0);
    v_Texture = a_Tex;
}