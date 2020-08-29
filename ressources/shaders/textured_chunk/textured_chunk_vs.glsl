#version 410 core

#define BLOCK_PER_LINE 16
#define LINE_PER_PLANE 16
#define PLANE_PER_CHUNK 256
#define BLOCK_PER_CHUNK BLOCK_PER_LINE * LINE_PER_PLANE * PLANE_PER_CHUNK

layout (location = 0) in uint block;

uniform vec3 uniform_vec_chunk_position;
uniform vec3 uniform_vec_camera_pos;

out VS_OUT {
    vec4 block_position;
    uint block_type;
    uint block_faces;
    float[6] backface_culling;
} vs_out;

float[6] compute_backface_culling(const vec4 block_center)
{
    float[6] result;
    vec3 view_to_block_center = gl_Position.xyz + block_center.xyz - uniform_vec_camera_pos;

    //XY-
    result[0] = dot(vec3(0.0f, 0.0f, -1.0f), (view_to_block_center + vec3(0.0f, 0.0f, -0.5f)));
    //XY+
    result[1] = dot(vec3(0.0f, 0.0f, 1.0f), (view_to_block_center + vec3(0.0f, 0.0f, 0.5f)));
    //XZ-
    result[2] = dot(vec3(0.0f, -1.0f, 0.0f), (view_to_block_center + vec3(0.0f, -0.5f, 0.0f)));
    //XZ+
    result[3] = dot(vec3(0.0f, 1.0f, 0.0f), (view_to_block_center + vec3(0.0f, 0.5f, 0.0f)));
    //YZ-
    result[4] = dot(vec3(-1.0f, 0.0f, 0.0f), (view_to_block_center + vec3(-0.5f, 0.0f, 0.0f)));
    //YZ+
    result[5] = dot(vec3(1.0f, 0.0f, 0.0f), (view_to_block_center + vec3(0.5f, 0.0f, 0.0f)));
    return (result);
}

void main()
{
    // Block position
    // 4294901760 = 1111 1111 1111 1111 0000 0000 0000 0000
    uint block_id = (block & 4294901760u) >> 16;
    uint plane_id = block_id / (BLOCK_PER_LINE * LINE_PER_PLANE);
    uint line_id = block_id % PLANE_PER_CHUNK % BLOCK_PER_LINE;
    uint col_id =  block_id % PLANE_PER_CHUNK / LINE_PER_PLANE;
    vs_out.block_position = vec4(line_id, plane_id, col_id, 0);

    // Block type
    // 31 = 0000 0000 0000 0000 0000 0000 0001 1111
    vs_out.block_type = block & 31u;

    // Block faces
    // 16128 = 0000 0000 0000 0000 0011 1111 0000 0000
    vs_out.block_faces = (block & 16128u) >> 8;
    gl_Position = vec4(uniform_vec_chunk_position, 1.0);

    // Backface culling
    vs_out.backface_culling = compute_backface_culling(vs_out.block_position);
}