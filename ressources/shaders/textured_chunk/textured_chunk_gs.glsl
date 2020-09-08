#version 410 core

layout (points) in;
layout (triangle_strip, max_vertices = 12) out;

#define TOTAL_TEX 16

struct BlockTextureType {
    vec2 id_xy_plus;
    vec2 id_xy_minus;
    vec2 id_xz_plus;
    vec2 id_xz_minus;
    vec2 id_yz_plus;
    vec2 id_yz_minus;
};

uniform mat4 uniform_mat_perspec_view;
layout (std140) uniform uniform_block_type
{
    BlockTextureType block_type[TOTAL_TEX];
};

in VS_OUT {
    vec4 block_position;
    uint block_type;
    uint block_faces;
    float[6] backface_culling;
} gs_in[];

out GS_OUT {
    vec2 base_texture_coord;
    float depth_z;
} gs_out;

// Texture should have 16x16 tiles
#define TEX_STEP 0.0625

// Texture offset
#define UP_RIGHT_OFF vec2(TEX_STEP, 0)
#define DOWN_LEFT_OFF vec2(0, TEX_STEP)
#define DOWN_RIGHT_OFF vec2(TEX_STEP, TEX_STEP)

void generate_textured_block(const vec4 block_center, const uint block_faces, const uint type,
const BlockTextureType tex_type, const float[6] backface_culling)
{
    //FACE XY-
    if ((block_faces & (1u << 5)) > 0 && backface_culling[0] < 0.0f) {
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, -0.5, -0.5, 0.0) + block_center);
        gs_out.depth_z = gl_Position.z;
        gs_out.base_texture_coord = tex_type.id_xy_minus + DOWN_RIGHT_OFF;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, -0.5, -0.5, 0.0) + block_center);
        gs_out.depth_z = gl_Position.z;
        gs_out.base_texture_coord = tex_type.id_xy_minus + DOWN_LEFT_OFF;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, 0.5, -0.5, 0.0) + block_center);
        gs_out.depth_z = gl_Position.z;
        gs_out.base_texture_coord = tex_type.id_xy_minus + UP_RIGHT_OFF;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, 0.5, -0.5, 0.0) + block_center);
        gs_out.depth_z = gl_Position.z;
        gs_out.base_texture_coord = tex_type.id_xy_minus;
        EmitVertex();
        EndPrimitive();
    }

    //FACE XY+
    if ((block_faces & (1u << 4)) > 0 && backface_culling[1] < 0.0f) {
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, 0.5, 0.5, 0.0) + block_center);
        gs_out.depth_z = gl_Position.z;
        gs_out.base_texture_coord = tex_type.id_xy_plus;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, 0.5, 0.5, 0.0) + block_center);
        gs_out.depth_z = gl_Position.z;
        gs_out.base_texture_coord = tex_type.id_xy_plus + UP_RIGHT_OFF;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, -0.5, 0.5, 0.0) + block_center);
        gs_out.depth_z = gl_Position.z;
        gs_out.base_texture_coord = tex_type.id_xy_plus + DOWN_LEFT_OFF;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, -0.5, 0.5, 0.0) + block_center);
        gs_out.depth_z = gl_Position.z;
        gs_out.base_texture_coord = tex_type.id_xy_plus + DOWN_RIGHT_OFF;
        EmitVertex();
        EndPrimitive();
    }

    //FACE XZ-
    if ((block_faces & (1u << 1)) > 0 && backface_culling[2] < 0.0f) {
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, -0.5, 0.5, 0.0) + block_center);
        gs_out.depth_z = gl_Position.z;
        gs_out.base_texture_coord = tex_type.id_xz_minus + DOWN_LEFT_OFF;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, -0.5, 0.5, 0.0) + block_center);
        gs_out.depth_z = gl_Position.z;
        gs_out.base_texture_coord = tex_type.id_xz_minus;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, -0.5, -0.5, 0.0) + block_center);
        gs_out.depth_z = gl_Position.z;
        gs_out.base_texture_coord = tex_type.id_xz_minus + DOWN_RIGHT_OFF;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, -0.5, -0.5, 0.0) + block_center);
        gs_out.depth_z = gl_Position.z;
        gs_out.base_texture_coord = tex_type.id_xz_minus + UP_RIGHT_OFF;
        EmitVertex();
        EndPrimitive();
    }

    //FACE XZ+
    if ((block_faces & 1u) > 0 && backface_culling[3] < 0.0f) {
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, 0.5, -0.5, 0.0) + block_center);
        gs_out.depth_z = gl_Position.z;
        gs_out.base_texture_coord = tex_type.id_xz_plus + UP_RIGHT_OFF;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, 0.5, -0.5, 0.0) + block_center);
        gs_out.depth_z = gl_Position.z;
        gs_out.base_texture_coord = tex_type.id_xz_plus + DOWN_RIGHT_OFF;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, 0.5, 0.5, 0.0) + block_center);
        gs_out.depth_z = gl_Position.z;
        gs_out.base_texture_coord = tex_type.id_xz_plus;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, 0.5, 0.5, 0.0) + block_center);
        gs_out.depth_z = gl_Position.z;
        gs_out.base_texture_coord = tex_type.id_xz_plus + DOWN_LEFT_OFF;
        EmitVertex();
        EndPrimitive();
    }

    //FACE YZ-
    if ((block_faces & (1u << 3)) > 0 && backface_culling[4] < 0.0f) {
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, -0.5, -0.5, 0.0) + block_center);
        gs_out.depth_z = gl_Position.z;
        gs_out.base_texture_coord = tex_type.id_yz_minus + DOWN_LEFT_OFF;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, 0.5, -0.5, 0.0) + block_center);
        gs_out.depth_z = gl_Position.z;
        gs_out.base_texture_coord = tex_type.id_yz_minus;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, -0.5, 0.5, 0.0) + block_center);
        gs_out.depth_z = gl_Position.z;
        gs_out.base_texture_coord = tex_type.id_yz_minus + DOWN_RIGHT_OFF;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, 0.5, 0.5, 0.0) + block_center);
        gs_out.depth_z = gl_Position.z;
        gs_out.base_texture_coord = tex_type.id_yz_minus + UP_RIGHT_OFF;
        EmitVertex();
        EndPrimitive();
    }

    //FACE YZ+
    if ((block_faces & (1u << 2)) > 0 && backface_culling[5] < 0.0f) {
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, -0.5, 0.5, 0.0) + block_center);
        gs_out.depth_z = gl_Position.z;
        gs_out.base_texture_coord = tex_type.id_yz_plus + DOWN_LEFT_OFF;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, 0.5, 0.5, 0.0) + block_center);
        gs_out.depth_z = gl_Position.z;
        gs_out.base_texture_coord = tex_type.id_yz_plus;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, -0.5, -0.5, 0.0) + block_center);
        gs_out.depth_z = gl_Position.z;
        gs_out.base_texture_coord = tex_type.id_yz_plus + DOWN_RIGHT_OFF;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, 0.5, -0.5, 0.0) + block_center);
        gs_out.depth_z = gl_Position.z;
        gs_out.base_texture_coord = tex_type.id_yz_plus + UP_RIGHT_OFF;
        EmitVertex();
        EndPrimitive();
    }
}

void main()
{
    uint type = gs_in[0].block_type - 1;
    generate_textured_block(gs_in[0].block_position, gs_in[0].block_faces, type,
    block_type[type], gs_in[0].backface_culling);
}
