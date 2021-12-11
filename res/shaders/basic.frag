#version 400 core
out vec4 f_Color;

in vec2 v_Texture;

uniform sampler2D tex0;

void main()
{
    // f_Color = texture(tex0, v_Texture);
    f_Color = vec4(v_Texture, 0.0, 1.0);
}