#version 330 core

layout(location = 0) in vec3 a_position;
// layout(location = 1) in vec3 a_color;
layout(location = 1) in vec2 a_texture;

// uniform mat4 rotation;
// uniform mat4 im_squash;

// out vec3 v_color;
out vec2 v_texture;
out vec3 v_position;

void main()
{
	// v_position = (im_squash * vec4(a_position, 1.0)).xyz;
	v_position = pos.xyz;
	// gl_Position = squash * rotation * vec4(a_position, 1.0);
	// v_color = a_color;
	v_texture = a_texture;
};
