#version 400 core
layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec2 a_Tex;

out vec2 v_Texture;

uniform mat4 u_WindowScale;
uniform mat4 u_FrameScale;

void main()
{
    gl_Position = u_WindowScale * u_FrameScale * vec4(a_Pos, 1.0);
    v_Texture = a_Tex;
}