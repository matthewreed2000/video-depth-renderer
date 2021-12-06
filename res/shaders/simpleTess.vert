#version 400 core
layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec2 a_Tex;

uniform mat4 u_MVP;

out vec3 vPosition;
 
void main()
{
    vPosition = a_Pos;
}