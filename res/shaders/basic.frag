#version 330 core
out vec4 f_Color;

in vec2 v_Texture;

void main()
{
    f_Color = vec4(v_Texture, 0.0f, 1.0f);
}