#version 410 core

layout (points) in;
layout (triangle_strip, max_vertices = 24) out;

#define TOTAL_TEX 16

struct BlockTextureType {
    vec2 id_xy_plus;
    vec2 id_xy_minus;
    vec2 id_xz_plus;
    vec2 id_xz_minus;
    vec2 id_yz_plus;
    vec2 id_yz_minus;
    vec2 level_1_texture_coord;
    int use_color_modifier;
    int unused_1;
};

uniform mat4 uniform_mat_perspec_view;
uniform vec4 uniform_vec_color_modifier;
uniform vec3 uniform_vec_camera_pos;
uniform float uniform_log_z_fighting;

layout (std140) uniform uniform_block_type
{
    BlockTextureType block_type[TOTAL_TEX];
};

in VS_OUT {
    vec4 block_position;
    uint block_type;
    uint block_faces;
} gs_in[];

out GS_OUT {
    vec2 base_texture_coord;
    vec2 level_1_texture_coord;
    vec4 color_modifier;
    float depth_z;
} gs_out;

#define NO_COLOR_MOD vec4(1.0)

// Texture should have 16x16 tiles
#define TEX_STEP 0.0625

// Texture offset
#define UP_RIGHT_OFF vec2(TEX_STEP, 0)
#define DOWN_LEFT_OFF vec2(0, TEX_STEP)
#define DOWN_RIGHT_OFF vec2(TEX_STEP, TEX_STEP)

// Constants for Z fighting
#define C  1.0
#define FAR 8000.0
#define OFFSET 1.0

void generate_textured_block(const vec4 block_center, const uint block_faces,
const BlockTextureType type, vec4 color_modifier, const float[6] backface_culling)
{
    if (type.use_color_modifier == 0) {
        color_modifier = NO_COLOR_MOD;
    }

    //FACE XY-
    if ((block_faces & (1u << 5)) > 0 && backface_culling[0] < 0.0f) {
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, -0.5, -0.5, 0.0) + block_center);
        gs_out.base_texture_coord = type.id_xy_minus + DOWN_RIGHT_OFF;
        gs_out.level_1_texture_coord = type.level_1_texture_coord;
        gs_out.color_modifier = color_modifier;
        gl_Position.z = 2.0 * log(gl_Position.w * C + OFFSET) * uniform_log_z_fighting - 1.0;
        gl_Position.z *= gl_Position.w;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, -0.5, -0.5, 0.0) + block_center);
        gs_out.base_texture_coord = type.id_xy_minus + DOWN_LEFT_OFF;
        gs_out.level_1_texture_coord = type.level_1_texture_coord;
        gs_out.color_modifier = color_modifier;
        gl_Position.z = 2.0 * log(gl_Position.w * C + OFFSET) * uniform_log_z_fighting - 1.0;
        gl_Position.z *= gl_Position.w;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, 0.5, -0.5, 0.0) + block_center);
        gs_out.base_texture_coord = type.id_xy_minus + UP_RIGHT_OFF;
        gs_out.level_1_texture_coord = type.level_1_texture_coord;
        gs_out.color_modifier = color_modifier;
        gl_Position.z = 2.0 * log(gl_Position.w * C + OFFSET) * uniform_log_z_fighting - 1.0;
        gl_Position.z *= gl_Position.w;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, 0.5, -0.5, 0.0) + block_center);
        gs_out.base_texture_coord = type.id_xy_minus;
        gs_out.level_1_texture_coord = type.level_1_texture_coord;
        gs_out.color_modifier = color_modifier;
        gl_Position.z = 2.0 * log(gl_Position.w * C + OFFSET) * uniform_log_z_fighting - 1.0;
        gl_Position.z *= gl_Position.w;
        EmitVertex();
        EndPrimitive();
    }

    //FACE XY+
    if ((block_faces & (1u << 4)) > 0 && backface_culling[1] < 0.0f) {
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, 0.5, 0.5, 0.0) + block_center);
        gs_out.base_texture_coord = type.id_xy_plus;
        gs_out.level_1_texture_coord = type.level_1_texture_coord;
        gs_out.color_modifier = color_modifier;
        gl_Position.z = 2.0 * log(gl_Position.w * C + OFFSET) * uniform_log_z_fighting - 1.0;
        gl_Position.z *= gl_Position.w;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, 0.5, 0.5, 0.0) + block_center);
        gs_out.base_texture_coord = type.id_xy_plus + UP_RIGHT_OFF;
        gs_out.level_1_texture_coord = type.level_1_texture_coord;
        gs_out.color_modifier = color_modifier;
        gl_Position.z = 2.0 * log(gl_Position.w * C + OFFSET) * uniform_log_z_fighting - 1.0;
        gl_Position.z *= gl_Position.w;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, -0.5, 0.5, 0.0) + block_center);
        gs_out.base_texture_coord = type.id_xy_plus + DOWN_LEFT_OFF;
        gs_out.level_1_texture_coord = type.level_1_texture_coord;
        gs_out.color_modifier = color_modifier;
        gl_Position.z = 2.0 * log(gl_Position.w * C + OFFSET) * uniform_log_z_fighting - 1.0;
        gl_Position.z *= gl_Position.w;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, -0.5, 0.5, 0.0) + block_center);
        gs_out.base_texture_coord = type.id_xy_plus + DOWN_RIGHT_OFF;
        gs_out.level_1_texture_coord = type.level_1_texture_coord;
        gs_out.color_modifier = color_modifier;
        gl_Position.z = 2.0 * log(gl_Position.w * C + OFFSET) * uniform_log_z_fighting - 1.0;
        gl_Position.z *= gl_Position.w;
        EmitVertex();
        EndPrimitive();
    }

    //FACE XZ-
    if ((block_faces & (1u << 1)) > 0 && backface_culling[2] < 0.0f) {
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, -0.5, 0.5, 0.0) + block_center);
        gs_out.base_texture_coord = type.id_xz_minus + DOWN_LEFT_OFF;
        gs_out.level_1_texture_coord = type.level_1_texture_coord;
        gs_out.color_modifier = color_modifier;
        gl_Position.z = 2.0 * log(gl_Position.w * C + OFFSET) * uniform_log_z_fighting - 1.0;
        gl_Position.z *= gl_Position.w;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, -0.5, 0.5, 0.0) + block_center);
        gs_out.base_texture_coord = type.id_xz_minus;
        gs_out.level_1_texture_coord = type.level_1_texture_coord;
        gs_out.color_modifier = color_modifier;
        gl_Position.z = 2.0 * log(gl_Position.w * C + OFFSET) * uniform_log_z_fighting - 1.0;
        gl_Position.z *= gl_Position.w;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, -0.5, -0.5, 0.0) + block_center);
        gs_out.base_texture_coord = type.id_xz_minus + DOWN_RIGHT_OFF;
        gs_out.level_1_texture_coord = type.level_1_texture_coord;
        gs_out.color_modifier = color_modifier;
        gl_Position.z = 2.0 * log(gl_Position.w * C + OFFSET) * uniform_log_z_fighting - 1.0;
        gl_Position.z *= gl_Position.w;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, -0.5, -0.5, 0.0) + block_center);
        gs_out.base_texture_coord = type.id_xz_minus + UP_RIGHT_OFF;
        gs_out.level_1_texture_coord = type.level_1_texture_coord;
        gs_out.color_modifier = color_modifier;
        gl_Position.z = 2.0 * log(gl_Position.w * C + OFFSET) * uniform_log_z_fighting - 1.0;
        gl_Position.z *= gl_Position.w;
        EmitVertex();
        EndPrimitive();
    }

    //FACE XZ+
    if ((block_faces & 1u) > 0 && backface_culling[3] < 0.0f) {
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, 0.5, -0.5, 0.0) + block_center);
        gs_out.base_texture_coord = type.id_xz_plus + UP_RIGHT_OFF;
        gs_out.level_1_texture_coord = type.level_1_texture_coord;
        gs_out.color_modifier = color_modifier;
        gl_Position.z = 2.0 * log(gl_Position.w * C + OFFSET) * uniform_log_z_fighting - 1.0;
        gl_Position.z *= gl_Position.w;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, 0.5, -0.5, 0.0) + block_center);
        gs_out.base_texture_coord = type.id_xz_plus + DOWN_RIGHT_OFF;
        gs_out.level_1_texture_coord = type.level_1_texture_coord;
        gs_out.color_modifier = color_modifier;
        gl_Position.z = 2.0 * log(gl_Position.w * C + OFFSET) * uniform_log_z_fighting - 1.0;
        gl_Position.z *= gl_Position.w;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, 0.5, 0.5, 0.0) + block_center);
        gs_out.base_texture_coord = type.id_xz_plus;
        gs_out.level_1_texture_coord = type.level_1_texture_coord;
        gs_out.color_modifier = color_modifier;
        gl_Position.z = 2.0 * log(gl_Position.w * C + OFFSET) * uniform_log_z_fighting - 1.0;
        gl_Position.z *= gl_Position.w;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, 0.5, 0.5, 0.0) + block_center);
        gs_out.base_texture_coord = type.id_xz_plus + DOWN_LEFT_OFF;
        gs_out.level_1_texture_coord = type.level_1_texture_coord;
        gs_out.color_modifier = color_modifier;
        gl_Position.z = 2.0 * log(gl_Position.w * C + OFFSET) * uniform_log_z_fighting - 1.0;
        gl_Position.z *= gl_Position.w;
        EmitVertex();
        EndPrimitive();
    }

    //FACE YZ-
    if ((block_faces & (1u << 3)) > 0 && backface_culling[4] < 0.0f) {
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, -0.5, -0.5, 0.0) + block_center);
        gs_out.base_texture_coord = type.id_yz_minus + DOWN_LEFT_OFF;
        gs_out.level_1_texture_coord = type.level_1_texture_coord;
        gs_out.color_modifier = color_modifier;
        gl_Position.z = 2.0 * log(gl_Position.w * C + OFFSET) * uniform_log_z_fighting - 1.0;
        gl_Position.z *= gl_Position.w;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, 0.5, -0.5, 0.0) + block_center);
        gs_out.base_texture_coord = type.id_yz_minus;
        gs_out.level_1_texture_coord = type.level_1_texture_coord;
        gs_out.color_modifier = color_modifier;
        gl_Position.z = 2.0 * log(gl_Position.w * C + OFFSET) * uniform_log_z_fighting - 1.0;
        gl_Position.z *= gl_Position.w;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, -0.5, 0.5, 0.0) + block_center);
        gs_out.base_texture_coord = type.id_yz_minus + DOWN_RIGHT_OFF;
        gs_out.level_1_texture_coord = type.level_1_texture_coord;
        gs_out.color_modifier = color_modifier;
        gl_Position.z = 2.0 * log(gl_Position.w * C + OFFSET) * uniform_log_z_fighting - 1.0;
        gl_Position.z *= gl_Position.w;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, 0.5, 0.5, 0.0) + block_center);
        gs_out.base_texture_coord = type.id_yz_minus + UP_RIGHT_OFF;
        gs_out.level_1_texture_coord = type.level_1_texture_coord;
        gs_out.color_modifier = color_modifier;
        gl_Position.z = 2.0 * log(gl_Position.w * C + OFFSET) * uniform_log_z_fighting - 1.0;
        gl_Position.z *= gl_Position.w;
        EmitVertex();
        EndPrimitive();
    }

    //FACE YZ+
    if ((block_faces & (1u << 2)) > 0 && backface_culling[5] < 0.0f) {
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, -0.5, 0.5, 0.0) + block_center);
        gs_out.base_texture_coord = type.id_yz_plus + DOWN_LEFT_OFF;
        gs_out.level_1_texture_coord = type.level_1_texture_coord;
        gs_out.color_modifier = color_modifier;
        gl_Position.z = 2.0 * log(gl_Position.w * C + OFFSET) * uniform_log_z_fighting - 1.0;
        gl_Position.z *= gl_Position.w;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, 0.5, 0.5, 0.0) + block_center);
        gs_out.base_texture_coord = type.id_yz_plus;
        gs_out.level_1_texture_coord = type.level_1_texture_coord;
        gs_out.color_modifier = color_modifier;
        gl_Position.z = 2.0 * log(gl_Position.w * C + OFFSET) * uniform_log_z_fighting - 1.0;
        gl_Position.z *= gl_Position.w;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, -0.5, -0.5, 0.0) + block_center);
        gs_out.base_texture_coord = type.id_yz_plus +  DOWN_RIGHT_OFF;
        gs_out.level_1_texture_coord = type.level_1_texture_coord;
        gs_out.color_modifier = color_modifier;
        gl_Position.z = 2.0 * log(gl_Position.w * C + OFFSET) * uniform_log_z_fighting - 1.0;
        gl_Position.z *= gl_Position.w;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, 0.5, -0.5, 0.0) + block_center);
        gs_out.base_texture_coord = type.id_yz_plus + UP_RIGHT_OFF;
        gs_out.level_1_texture_coord = type.level_1_texture_coord;
        gs_out.color_modifier = color_modifier;
        gl_Position.z = 2.0 * log(gl_Position.w * C + OFFSET) * uniform_log_z_fighting - 1.0;
        gl_Position.z *= gl_Position.w;
        EmitVertex();
        EndPrimitive();
    }
}

float[6] compute_backface_culling_tests(const vec4 block_center)
{
    float[6] result;
    vec3 view_to_block_center = gl_in[0].gl_Position.xyz + block_center.xyz - uniform_vec_camera_pos;

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
    uint type = gs_in[0].block_type - 1;
    float[6] backface_culling = compute_backface_culling_tests(gs_in[0].block_position);
    generate_textured_block(gs_in[0].block_position, gs_in[0].block_faces, block_type[type],
    uniform_vec_color_modifier, backface_culling);
}