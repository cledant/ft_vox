#version 410 core

#define BLOCK_PER_LINE 16
#define LINE_PER_PLANE 16
#define PLANE_PER_CHUNK 256
#define BLOCK_PER_CHUNK BLOCK_PER_LINE * LINE_PER_PLANE * PLANE_PER_CHUNK

#define TOTAL_TEX 16
#define NO_COLOR_MOD vec4(1.0)

// Texture should have 16x16 tiles
#define TEX_STEP 0.0625

// Texture offset
#define UP_RIGHT_OFF vec2(TEX_STEP, 0)
#define DOWN_LEFT_OFF vec2(0, TEX_STEP)
#define DOWN_RIGHT_OFF vec2(TEX_STEP, TEX_STEP)

struct BlockTextureType {
    vec4 tex_coord[6];
};

layout (std140) uniform uniform_block_tex
{
    BlockTextureType block_tex[TOTAL_TEX];
};
uniform mat4 uniform_mat_perspec_view;
uniform vec3 uniform_vec_chunk_position;
uniform vec3 uniform_vec_camera_pos;

out VS_OUT {
    vec2 base_texture_coord;
    float depth_z;
    float to_discard;
} vs_out;

layout (location = 0) in uint block;

const vec3 normals[6] = vec3[6](
//XZ+
vec3(0.0f, 1.0f, 0.0f),
//XZ-
vec3(0.0f, -1.0f, 0.0f),
//YZ+
vec3(1.0f, 0.0f, 0.0f),
//YZ-
vec3(-1.0f, 0.0f, 0.0f),
//XY+
vec3(0.0f, 0.0f, 1.0f),
//XY-
vec3(0.0f, 0.0f, -1.0f));

const vec3 offsets[6] = vec3[6](
//XZ+
vec3(0.0f, 0.5f, 0.0f),
//XZ-
vec3(0.0f, -0.5f, 0.0f),
//YZ+
vec3(0.5f, 0.0f, 0.0f),
//YZ-
vec3(-0.5f, 0.0f, 0.0f),
//XY+
vec3(0.0f, 0.0f, 0.5f),
//XY-
vec3(0.0f, 0.0f, -0.5f));

const vec4 vertex[24] = vec4[24](
//FACE XZ+
vec4(-0.5, 0.5, -0.5, 0.0),
vec4(0.5, 0.5, -0.5, 0.0),
vec4(-0.5, 0.5, 0.5, 0.0),
vec4(0.5, 0.5, 0.5, 0.0),
//FACE XZ-
vec4(-0.5, -0.5, 0.5, 0.0),
vec4(0.5, -0.5, 0.5, 0.0),
vec4(-0.5, -0.5, -0.5, 0.0),
vec4(0.5, -0.5, -0.5, 0.0),
//FACE YZ+
vec4(0.5, -0.5, 0.5, 0.0),
vec4(0.5, 0.5, 0.5, 0.0),
vec4(0.5, -0.5, -0.5, 0.0),
vec4(0.5, 0.5, -0.5, 0.0),
//FACE YZ-
vec4(-0.5, -0.5, -0.5, 0.0),
vec4(-0.5, 0.5, -0.5, 0.0),
vec4(-0.5, -0.5, 0.5, 0.0),
vec4(-0.5, 0.5, 0.5, 0.0),
//FACE XY+
vec4(-0.5, 0.5, 0.5, 0.0),
vec4(0.5, 0.5, 0.5, 0.0),
vec4(-0.5, -0.5, 0.5, 0.0),
vec4(0.5, -0.5, 0.5, 0.0),
//FACE XY-
vec4(-0.5, -0.5, -0.5, 0.0),
vec4(0.5, -0.5, -0.5, 0.0),
vec4(-0.5, 0.5, -0.5, 0.0),
vec4(0.5, 0.5, -0.5, 0.0));

const vec2 texture_coord_offset[24] = vec2[24](
//FACE XZ+
vec2(UP_RIGHT_OFF),
vec2(DOWN_RIGHT_OFF),
vec2(0.0),
vec2(DOWN_LEFT_OFF),
//FACE XZ-
vec2(DOWN_LEFT_OFF),
vec2(0.0),
vec2(DOWN_RIGHT_OFF),
vec2(UP_RIGHT_OFF),
//FACE YZ+
vec2(DOWN_LEFT_OFF),
vec2(0.0),
vec2(DOWN_RIGHT_OFF),
vec2(UP_RIGHT_OFF),
//FACE YZ-
vec2(DOWN_LEFT_OFF),
vec2(0.0),
vec2(DOWN_RIGHT_OFF),
vec2(UP_RIGHT_OFF),
//FACE XY+
vec2(0.0),
vec2(UP_RIGHT_OFF),
vec2(DOWN_LEFT_OFF),
vec2(DOWN_RIGHT_OFF),
//FACE XY-
vec2(DOWN_RIGHT_OFF),
vec2(DOWN_LEFT_OFF),
vec2(UP_RIGHT_OFF),
vec2(0.0));

void main()
{
    // Block faces
    // 16128 = 0000 0000 0000 0000 0011 1111 0000 0000
    uint block_faces = (block & 16128u) >> 8;
    vec4 chunk_position = vec4(uniform_vec_chunk_position, 1.0);

    // Block position
    // 4294901760 = 1111 1111 1111 1111 0000 0000 0000 0000
    uint block_id = (block & 4294901760u) >> 16;
    uint plane_id = block_id / (BLOCK_PER_LINE * LINE_PER_PLANE);
    uint line_id = block_id % PLANE_PER_CHUNK % BLOCK_PER_LINE;
    uint col_id =  block_id % PLANE_PER_CHUNK / LINE_PER_PLANE;
    vec4 block_position = vec4(line_id, plane_id, col_id, 0);

    // Should face be displayed
    uint instance_face = gl_InstanceID % 6;
    float backface_culling =
    dot(normals[instance_face],
    (chunk_position.xyz + block_position.xyz - uniform_vec_camera_pos + offsets[instance_face]));
    if ((block_faces & (1u << instance_face)) == 0 || backface_culling >= 0.0f) {
        vs_out.to_discard = 1.0;
        gl_Position = vec4(0.0);
        return;
    }
    vs_out.to_discard = 0.0;

    // Vertex
    uint vertex_idx = gl_VertexID + instance_face * 4;
    gl_Position = uniform_mat_perspec_view * (chunk_position + vertex[vertex_idx] + block_position);
    vs_out.depth_z = gl_Position.z;

    // Texture
    // Block type
    // 31 = 0000 0000 0000 0000 0000 0000 0001 1111
    uint block_type = block & 31u;
    vs_out.base_texture_coord = block_tex[block_type - 1].tex_coord[instance_face].xy + texture_coord_offset[vertex_idx];
}