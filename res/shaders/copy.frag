#version 330 core

in vec3 te_color;
in vec2 te_texture;

out vec4 color;

uniform sampler2D tex0;

void main()
{
	// color = vec4(te_color, 1.0);
	color = texture(tex0, te_texture);
	// color = vec4(te_texture, 0.0, 1.0);
	// color = vec4(0.0, 0.0, 1.0, 1.0);
};