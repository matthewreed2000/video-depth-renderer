#version 330 core
layout (location = 0) in vec3 v_Pos;
layout (location = 1) in vec2 v_Tex;

out vec3 tc_Pos;
out vec2 tc_Tex;

void main()
{
    tc_Pos = v_Pos;
    tc_Tex = vec2(v_Tex.x * 0.5f + 0.5f, v_Tex.y);
}