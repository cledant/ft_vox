#version 410 core

layout (location = 0) in vec3 player_pos;

void main()
{
    gl_Position = vec4(player_pos, 1.0);
}