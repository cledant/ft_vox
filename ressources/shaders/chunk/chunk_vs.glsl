#version 410 core

#define BLOCK_PER_LINE 16
#define LINE_PER_PLANE 16
#define PLANE_PER_CHUNK 256
#define BLOCK_PER_CHUNK BLOCK_PER_LINE * LINE_PER_PLANE * PLANE_PER_CHUNK

layout (location = 0) in uint block;

uniform vec3 uniform_vec_chunk_position;
uniform vec4 uniform_mat_frustum_plane[6];
uniform vec4 uniform_mat_abs_frustum_plane[6];

const vec4 BLOCK_EXTENT = vec4(1.0);

out VS_OUT {
    vec4 block_position;
    uint block;
    uint visible;
} vs_out;

int isBoxVisible(vec4 center, vec4 frustum_plane[6], vec4 abs_frustum_plane[6])
{
    for (uint i = 0; i < 6; ++i) {
        float d = dot(center.xyz, frustum_plane[i].xyz);
        float r = dot(BLOCK_EXTENT.xyz, abs_frustum_plane[i].xyz);
        if (d + r < -frustum_plane[i].w) {
            return (0);
        }
    }
    return (1);
}

void main()
{
    int plane_id = gl_InstanceID / (BLOCK_PER_LINE * LINE_PER_PLANE);
    int line_id = gl_InstanceID % PLANE_PER_CHUNK % BLOCK_PER_LINE;
    int col_id =  gl_InstanceID % PLANE_PER_CHUNK / LINE_PER_PLANE;

    vs_out.block = block;
    vs_out.block_position = vec4(line_id, plane_id, col_id, 0);
    gl_Position = vec4(uniform_vec_chunk_position, 1.0);
    vs_out.visible = isBoxVisible(gl_Position + vs_out.block_position,
        uniform_mat_frustum_plane,
        uniform_mat_abs_frustum_plane);
}