#version 410 core

layout (points) in;
layout (triangle_strip, max_vertices = 24) out;

uniform mat4 uniform_mat_perspec_view;

in VS_OUT {
    vec4 block_position;
    int block_type;
    int block_faces;
} gs_in[];

out GS_OUT {
    vec2 texture_coord;
    vec4 color_modifier;
} gs_out;

#define NO_COLOR_MOD vec4(1.0)

// Texture should have 16x16 tiles
#define TEX_STEP 0.0625

// Texture offset
#define UP_RIGHT_OFF vec2(TEX_STEP, 0)
#define DOWN_LEFT_OFF vec2(0, TEX_STEP)
#define DOWN_RIGHT_OFF vec2(TEX_STEP, TEX_STEP)

// Up left texture coordinate
#define STONE vec2(TEX_STEP, 0)
#define GRASS_TOP vec2(1 * TEX_STEP, 0)
#define GRASS_SIDE vec2(3 * TEX_STEP, 0)
#define DIRT vec2(2 * TEX_STEP, 0)
#define COBBLESTONE vec2(0, TEX_STEP)

void generate_textured_block(vec4 block_center, int block_faces, vec2 id_xy_plus, vec2 id_xy_minus, vec2 id_xz_plus,
vec2 id_xz_minus, vec2 id_yz_plus, vec2 id_yz_minus)
{
    //FACE XY-
    if ((block_faces & (1 << 5)) > 0) {
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, -0.5, -0.5, 0.0) + block_center);
        gs_out.texture_coord = id_xy_minus + DOWN_RIGHT_OFF;
        gs_out.color_modifier = NO_COLOR_MOD;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, -0.5, -0.5, 0.0) + block_center);
        gs_out.texture_coord = id_xy_minus + DOWN_LEFT_OFF;
        gs_out.color_modifier = NO_COLOR_MOD;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, 0.5, -0.5, 0.0) + block_center);
        gs_out.texture_coord = id_xy_minus + UP_RIGHT_OFF;
        gs_out.color_modifier = NO_COLOR_MOD;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, 0.5, -0.5, 0.0) + block_center);
        gs_out.texture_coord = id_xy_minus;
        gs_out.color_modifier = NO_COLOR_MOD;
        EmitVertex();
        EndPrimitive();
    }

    //FACE XY+
    if ((block_faces & (1 << 4)) > 0) {
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, 0.5, 0.5, 0.0) + block_center);
        gs_out.texture_coord = id_xy_plus;
        gs_out.color_modifier = NO_COLOR_MOD;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, 0.5, 0.5, 0.0) + block_center);
        gs_out.texture_coord = id_xy_plus + UP_RIGHT_OFF;
        gs_out.color_modifier = NO_COLOR_MOD;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, -0.5, 0.5, 0.0) + block_center);
        gs_out.texture_coord = id_xy_plus + DOWN_LEFT_OFF;
        gs_out.color_modifier = NO_COLOR_MOD;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, -0.5, 0.5, 0.0) + block_center);
        gs_out.texture_coord = id_xy_plus + DOWN_RIGHT_OFF;
        gs_out.color_modifier = NO_COLOR_MOD;
        EmitVertex();
        EndPrimitive();
    }

    //FACE XZ-
    if ((block_faces & (1 << 1)) > 0) {
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, -0.5, 0.5, 0.0) + block_center);
        gs_out.texture_coord = id_xz_minus + DOWN_LEFT_OFF;
        gs_out.color_modifier = NO_COLOR_MOD;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, -0.5, 0.5, 0.0) + block_center);
        gs_out.texture_coord = id_xz_minus;
        gs_out.color_modifier = NO_COLOR_MOD;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, -0.5, -0.5, 0.0) + block_center);
        gs_out.texture_coord = id_xz_minus + DOWN_RIGHT_OFF;
        gs_out.color_modifier = NO_COLOR_MOD;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, -0.5, -0.5, 0.0) + block_center);
        gs_out.texture_coord = id_xz_minus + UP_RIGHT_OFF;
        gs_out.color_modifier = NO_COLOR_MOD;
        EmitVertex();
        EndPrimitive();
    }

    //FACE XZ+
    if ((block_faces & 1) > 0) {
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, 0.5, -0.5, 0.0) + block_center);
        gs_out.texture_coord = id_xz_plus + UP_RIGHT_OFF;
        gs_out.color_modifier = NO_COLOR_MOD;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, 0.5, -0.5, 0.0) + block_center);
        gs_out.texture_coord = id_xz_plus + DOWN_RIGHT_OFF;
        gs_out.color_modifier = NO_COLOR_MOD;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, 0.5, 0.5, 0.0) + block_center);
        gs_out.texture_coord = id_xz_plus;
        gs_out.color_modifier = NO_COLOR_MOD;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, 0.5, 0.5, 0.0) + block_center);
        gs_out.texture_coord = id_xz_plus + DOWN_LEFT_OFF;
        gs_out.color_modifier = NO_COLOR_MOD;
        EmitVertex();
        EndPrimitive();
    }

    //FACE YZ-
    if ((block_faces & (1 << 3)) > 0) {
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, -0.5, -0.5, 0.0) + block_center);
        gs_out.texture_coord = id_yz_minus + DOWN_LEFT_OFF;
        gs_out.color_modifier = NO_COLOR_MOD;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, 0.5, -0.5, 0.0) + block_center);
        gs_out.texture_coord = id_yz_minus;
        gs_out.color_modifier = NO_COLOR_MOD;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, -0.5, 0.5, 0.0) + block_center);
        gs_out.texture_coord = id_yz_minus + DOWN_RIGHT_OFF;
        gs_out.color_modifier = NO_COLOR_MOD;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, 0.5, 0.5, 0.0) + block_center);
        gs_out.texture_coord = id_yz_minus + UP_RIGHT_OFF;
        gs_out.color_modifier = NO_COLOR_MOD;
        EmitVertex();
        EndPrimitive();
    }

    //FACE YZ+
    if ((block_faces & (1 << 2)) > 0) {
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, -0.5, 0.5, 0.0) + block_center);
        gs_out.texture_coord = id_yz_plus + DOWN_LEFT_OFF;
        gs_out.color_modifier = NO_COLOR_MOD;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, 0.5, 0.5, 0.0) + block_center);
        gs_out.texture_coord = id_yz_plus;
        gs_out.color_modifier = NO_COLOR_MOD;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, -0.5, -0.5, 0.0) + block_center);
        gs_out.texture_coord = id_yz_plus +  DOWN_RIGHT_OFF;
        gs_out.color_modifier = NO_COLOR_MOD;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, 0.5, -0.5, 0.0) + block_center);
        gs_out.texture_coord = id_yz_plus + UP_RIGHT_OFF;
        gs_out.color_modifier = NO_COLOR_MOD;
        EmitVertex();
        EndPrimitive();
    }
}

void main()
{
    switch (gs_in[0].block_type) {
        case 1:
        generate_textured_block(gs_in[0].block_position, gs_in[0].block_faces, STONE, STONE, STONE, STONE, STONE, STONE);
        case 2:
        generate_textured_block(gs_in[0].block_position, gs_in[0].block_faces, GRASS_SIDE, GRASS_SIDE, GRASS_SIDE,
        GRASS_SIDE, GRASS_SIDE, GRASS_SIDE);
        case 3:
        generate_textured_block(gs_in[0].block_position, gs_in[0].block_faces, DIRT, DIRT, DIRT, DIRT, DIRT, DIRT);
        case 4:
        generate_textured_block(gs_in[0].block_position, gs_in[0].block_faces, COBBLESTONE, COBBLESTONE, COBBLESTONE,
        COBBLESTONE, COBBLESTONE, COBBLESTONE);
        default :
        return;
    }

}