#version 410 core

#define BLOCK_PER_LINE 16
#define LINE_PER_PLANE 16
#define PLANE_PER_CHUNK 256
#define BLOCK_PER_CHUNK BLOCK_PER_LINE * LINE_PER_PLANE * PLANE_PER_CHUNK

layout (location = 0) in uint chunk_block;
layout (location = 1) in ivec2 chunk_position;

out VS_OUT {
    uint chunk_block;
} vs_out;

void main()
{
    vs_out.chunk_block = chunk_block;
    gl_Position = vec4(vec3(chunk_position.x * BLOCK_PER_LINE, chunk_position.y * LINE_PER_PLANE, 0.0), 1.0);
}