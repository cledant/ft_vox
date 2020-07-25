#version 410 core

layout (location = 0) in vec4 vertex;

uniform mat4 uniform_mat_ortho;

out vec2 vertex_tex;

// Constants for Z fighting
#define C  1.0
#define FAR 8000.0
#define OFFSET 1.0
const float log_result = 1 / log(C * FAR + OFFSET);

void main (void)
{
    vertex_tex = vertex.zw;
    gl_Position = uniform_mat_ortho * vec4(vertex.xy, 0.0f, 1.0f);
    gl_Position.z = 2.0 * log(gl_Position.w * C + OFFSET) * log_result - 1.0;
    gl_Position.z *= gl_Position.w;
}