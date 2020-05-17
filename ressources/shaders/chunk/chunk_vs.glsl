#version 410 core

#define BLOCK_PER_LINE 16
#define LINE_PER_PLANE 16
#define PLANE_PER_CHUNK 256
#define BLOCK_PER_CHUNK BLOCK_PER_LINE * LINE_PER_PLANE * PLANE_PER_CHUNK

layout (location = 0) in int block;

uniform vec3 uniform_vec_chunk_position;

out VS_OUT {
    vec4 block_position;
    int block_type;
    int block_faces;
} vs_out;

void main()
{
    // Block position
    // 4294901760 = 1111 1111 1111 1111 0000 0000 0000 0000
    int block_id = (block & 4294901760) >> 16;
    int plane_id = block_id / (BLOCK_PER_LINE * LINE_PER_PLANE);
    int line_id = block_id % PLANE_PER_CHUNK % BLOCK_PER_LINE;
    int col_id =  block_id % PLANE_PER_CHUNK / LINE_PER_PLANE;
    vs_out.block_position = vec4(line_id, plane_id, col_id, 0);

    // Block type
    // 31 = 0000 0000 0000 0000 0000 0000 0001 1111
    vs_out.block_type = block & 31;

    // Block faces
    // 16128 = 0000 0000 0000 0000 0011 1111 0000 0000
    vs_out.block_faces = (block & 16128) >> 8;
    gl_Position = vec4(uniform_vec_chunk_position, 1.0);
}