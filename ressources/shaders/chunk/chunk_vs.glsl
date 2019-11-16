#version 410 core

#define BLOCK_PER_LINE 16
#define LINE_PER_PLANE 16
#define PLANE_PER_CHUNK 256
#define BLOCK_PER_CHUNK BLOCK_PER_LINE * LINE_PER_PLANE * PLANE_PER_CHUNK

layout (location = 0) in uint block;

uniform vec3 uniform_vec_chunk_position;

out VS_OUT {
    vec4 block_position;
    uint block;
} vs_out;

void main()
{
    int plane_id = gl_InstanceID / (BLOCK_PER_LINE * LINE_PER_PLANE);
    int line_id = gl_InstanceID % PLANE_PER_CHUNK % BLOCK_PER_LINE;
    int col_id =  gl_InstanceID % PLANE_PER_CHUNK / LINE_PER_PLANE;

    vs_out.block = block;
    vs_out.block_position = vec4(line_id, plane_id, col_id, 0);
    gl_Position = vec4(uniform_vec_chunk_position, 1.0);
}