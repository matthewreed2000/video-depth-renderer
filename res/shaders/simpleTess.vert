#version 330 core
layout (location = 0) in vec3 v_Pos;
layout (location = 1) in vec2 v_Tex;

out vec3 tc_Pos;
out vec2 tc_Tex;

// out vec3 f_Pos;
// out vec2 f_Tex;

void main()
{
    tc_Pos = v_Pos;
    tc_Tex = v_Tex;

    // f_Pos = (u_MVP * vec4(v_Pos, 1.0)).xyz;
    // f_Tex = v_Tex;
    // gl_Position = u_MVP * vec4(v_Pos, 1.0);
}