#version 330 core

out vec4 o_Color;

in vec3 f_Pos;
in vec2 f_Tex;

uniform sampler2D tex0;
// uniform sampler2D tex1;

void main()
{
    o_Color = texture(tex0, f_Tex);
}