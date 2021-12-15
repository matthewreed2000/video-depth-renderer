#version 330 core

out vec4 o_Color;

in vec3 f_Pos;
in vec2 f_Tex;

uniform sampler2D tex0;
// uniform sampler2D tex1;

void main()
{
    o_Color = texture(tex0, f_Tex);
    // o_Color = texture(tex1, f_Tex);
    // o_Color = vec4(f_Tex, 0.0, 1.0);
    // o_Color = vec4(f_Pos.xyz, 1.0);
    // o_Color = vec4(1.0, 1.0, 1.0, 1.0);

    // gl_FragColor = vec4(1.0,1.0,1.0,1.0);
}