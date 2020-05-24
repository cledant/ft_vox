#version 410 core

layout (points) in;
layout (triangle_strip, max_vertices = 24) out;

uniform mat4 uniform_mat_perspec_view;
uniform vec4 uniform_vec_color_modifier;

in VS_OUT {
    vec4 block_position;
    int block_type;
    int block_faces;
} gs_in[];

out GS_OUT {
    vec2 base_texture_coord;
    vec2 level_1_texture_coord;
    vec4 color_modifier;
} gs_out;

#define NO_COLOR_MOD vec4(1.0)

// Texture should have 16x16 tiles
#define TEX_STEP 0.0625

// Texture offset
#define UP_RIGHT_OFF vec2(TEX_STEP, 0)
#define DOWN_LEFT_OFF vec2(0, TEX_STEP)
#define DOWN_RIGHT_OFF vec2(TEX_STEP, TEX_STEP)

#define TOTAL_TEX 16

// Up left texture coordinate
#define STONE vec2(TEX_STEP, 0)
#define GRASS_TOP vec2(0, 0)
#define GRASS_SIDE vec2(6 * TEX_STEP, 2 * TEX_STEP)
#define DIRT vec2(2 * TEX_STEP, 0)
#define COBBLESTONE vec2(TEX_STEP, 0)
#define PLANK vec2(4 * TEX_STEP, 0)
#define BEDROCK vec2(TEX_STEP, TEX_STEP)
#define SAND vec2(2 * TEX_STEP, TEX_STEP)
#define WATER vec2(0, 9 * TEX_STEP)
#define SNOW vec2(2 * TEX_STEP, 4 * TEX_STEP)
#define SNOW_GRASS vec2(4 * TEX_STEP, 4 * TEX_STEP)
#define WOOD_TOP_BOT vec2(5 * TEX_STEP, TEX_STEP)
#define WOOD_SIDE vec2(4 * TEX_STEP, TEX_STEP)
#define FOLIAGE vec2(4 * TEX_STEP, 3 * TEX_STEP)
#define TNT_SIDE vec2(8 * TEX_STEP, 0)
#define TNT_TOP vec2(9 * TEX_STEP, 0)
#define TNT_BOT vec2(10 * TEX_STEP, 0)
#define BRICKS vec2(7 * TEX_STEP, 0)
#define GLASS vec2(1 * TEX_STEP, 3 * TEX_STEP)
#define ICE vec2(3 * TEX_STEP, 4 * TEX_STEP)

#define NO_TEXTURE vec2(2.0)

void generate_grass_block(vec4 block_center, int block_faces, vec4 color_modifier)
{
    //FACE XY-
    if ((block_faces & (1 << 5)) > 0) {
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, -0.5, -0.5, 0.0) + block_center);
        gs_out.base_texture_coord = DIRT + DOWN_RIGHT_OFF;
        gs_out.level_1_texture_coord = GRASS_SIDE + DOWN_RIGHT_OFF;
        gs_out.color_modifier = color_modifier;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, -0.5, -0.5, 0.0) + block_center);
        gs_out.base_texture_coord = DIRT + DOWN_LEFT_OFF;
        gs_out.level_1_texture_coord = GRASS_SIDE + DOWN_LEFT_OFF;
        gs_out.color_modifier = color_modifier;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, 0.5, -0.5, 0.0) + block_center);
        gs_out.base_texture_coord = DIRT + UP_RIGHT_OFF;
        gs_out.level_1_texture_coord = GRASS_SIDE + UP_RIGHT_OFF;
        gs_out.color_modifier = color_modifier;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, 0.5, -0.5, 0.0) + block_center);
        gs_out.base_texture_coord = DIRT;
        gs_out.level_1_texture_coord = GRASS_SIDE;
        gs_out.color_modifier = color_modifier;
        EmitVertex();
        EndPrimitive();
    }

    //FACE XY+
    if ((block_faces & (1 << 4)) > 0) {
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, 0.5, 0.5, 0.0) + block_center);
        gs_out.base_texture_coord = DIRT;
        gs_out.level_1_texture_coord = GRASS_SIDE;
        gs_out.color_modifier = color_modifier;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, 0.5, 0.5, 0.0) + block_center);
        gs_out.base_texture_coord = DIRT + UP_RIGHT_OFF;
        gs_out.level_1_texture_coord = GRASS_SIDE + UP_RIGHT_OFF;
        gs_out.color_modifier = color_modifier;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, -0.5, 0.5, 0.0) + block_center);
        gs_out.base_texture_coord = DIRT + DOWN_LEFT_OFF;
        gs_out.level_1_texture_coord = GRASS_SIDE + DOWN_LEFT_OFF;
        gs_out.color_modifier = color_modifier;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, -0.5, 0.5, 0.0) + block_center);
        gs_out.base_texture_coord = DIRT + DOWN_RIGHT_OFF;
        gs_out.level_1_texture_coord = GRASS_SIDE + DOWN_RIGHT_OFF;
        gs_out.color_modifier = color_modifier;
        EmitVertex();
        EndPrimitive();
    }

    //FACE XZ-
    if ((block_faces & (1 << 1)) > 0) {
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, -0.5, 0.5, 0.0) + block_center);
        gs_out.base_texture_coord = DIRT + DOWN_LEFT_OFF;
        gs_out.level_1_texture_coord = NO_TEXTURE;
        gs_out.color_modifier = color_modifier;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, -0.5, 0.5, 0.0) + block_center);
        gs_out.base_texture_coord = DIRT;
        gs_out.level_1_texture_coord = NO_TEXTURE;
        gs_out.color_modifier = color_modifier;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, -0.5, -0.5, 0.0) + block_center);
        gs_out.base_texture_coord = DIRT + DOWN_RIGHT_OFF;
        gs_out.level_1_texture_coord = NO_TEXTURE;
        gs_out.color_modifier = color_modifier;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, -0.5, -0.5, 0.0) + block_center);
        gs_out.base_texture_coord = DIRT + UP_RIGHT_OFF;
        gs_out.level_1_texture_coord = NO_TEXTURE;
        gs_out.color_modifier = color_modifier;
        EmitVertex();
        EndPrimitive();
    }

    //FACE XZ+
    if ((block_faces & 1) > 0) {
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, 0.5, -0.5, 0.0) + block_center);
        gs_out.base_texture_coord = GRASS_TOP + UP_RIGHT_OFF;
        gs_out.level_1_texture_coord = NO_TEXTURE;
        gs_out.color_modifier = color_modifier;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, 0.5, -0.5, 0.0) + block_center);
        gs_out.base_texture_coord = GRASS_TOP + DOWN_RIGHT_OFF;
        gs_out.level_1_texture_coord = NO_TEXTURE;
        gs_out.color_modifier = color_modifier;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, 0.5, 0.5, 0.0) + block_center);
        gs_out.base_texture_coord = GRASS_TOP;
        gs_out.level_1_texture_coord = NO_TEXTURE;
        gs_out.color_modifier = color_modifier;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, 0.5, 0.5, 0.0) + block_center);
        gs_out.base_texture_coord = GRASS_TOP + DOWN_LEFT_OFF;
        gs_out.level_1_texture_coord = NO_TEXTURE;
        gs_out.color_modifier = color_modifier;
        EmitVertex();
        EndPrimitive();
    }

    //FACE YZ-
    if ((block_faces & (1 << 3)) > 0) {
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, -0.5, -0.5, 0.0) + block_center);
        gs_out.base_texture_coord = DIRT + DOWN_LEFT_OFF;
        gs_out.level_1_texture_coord = GRASS_SIDE + DOWN_LEFT_OFF;
        gs_out.color_modifier = color_modifier;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, 0.5, -0.5, 0.0) + block_center);
        gs_out.base_texture_coord = DIRT;
        gs_out.level_1_texture_coord = GRASS_SIDE;
        gs_out.color_modifier = color_modifier;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, -0.5, 0.5, 0.0) + block_center);
        gs_out.base_texture_coord = DIRT + DOWN_RIGHT_OFF;
        gs_out.level_1_texture_coord = GRASS_SIDE + DOWN_RIGHT_OFF;
        gs_out.color_modifier = color_modifier;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, 0.5, 0.5, 0.0) + block_center);
        gs_out.base_texture_coord = DIRT + UP_RIGHT_OFF;
        gs_out.level_1_texture_coord = GRASS_SIDE + UP_RIGHT_OFF;
        gs_out.color_modifier = color_modifier;
        EmitVertex();
        EndPrimitive();
    }

    //FACE YZ+
    if ((block_faces & (1 << 2)) > 0) {
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, -0.5, 0.5, 0.0) + block_center);
        gs_out.base_texture_coord = DIRT + DOWN_LEFT_OFF;
        gs_out.level_1_texture_coord = GRASS_SIDE + DOWN_LEFT_OFF;
        gs_out.color_modifier = color_modifier;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, 0.5, 0.5, 0.0) + block_center);
        gs_out.base_texture_coord = DIRT;
        gs_out.level_1_texture_coord = GRASS_SIDE;
        gs_out.color_modifier = color_modifier;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, -0.5, -0.5, 0.0) + block_center);
        gs_out.base_texture_coord = DIRT +  DOWN_RIGHT_OFF;
        gs_out.level_1_texture_coord = GRASS_SIDE +  DOWN_RIGHT_OFF;
        gs_out.color_modifier = color_modifier;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, 0.5, -0.5, 0.0) + block_center);
        gs_out.base_texture_coord = DIRT + UP_RIGHT_OFF;
        gs_out.level_1_texture_coord = GRASS_SIDE + UP_RIGHT_OFF;
        gs_out.color_modifier = color_modifier;
        EmitVertex();
        EndPrimitive();
    }
}

void generate_textured_block(vec4 block_center, int block_faces, vec2 id_xy_plus, vec2 id_xy_minus, vec2 id_xz_plus,
vec2 id_xz_minus, vec2 id_yz_plus, vec2 id_yz_minus, vec4 color_modifier)
{
    //FACE XY-
    if ((block_faces & (1 << 5)) > 0) {
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, -0.5, -0.5, 0.0) + block_center);
        gs_out.base_texture_coord = id_xy_minus + DOWN_RIGHT_OFF;
        gs_out.level_1_texture_coord = NO_TEXTURE;
        gs_out.color_modifier = color_modifier;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, -0.5, -0.5, 0.0) + block_center);
        gs_out.base_texture_coord = id_xy_minus + DOWN_LEFT_OFF;
        gs_out.level_1_texture_coord = NO_TEXTURE;
        gs_out.color_modifier = color_modifier;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, 0.5, -0.5, 0.0) + block_center);
        gs_out.base_texture_coord = id_xy_minus + UP_RIGHT_OFF;
        gs_out.level_1_texture_coord = NO_TEXTURE;
        gs_out.color_modifier = color_modifier;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, 0.5, -0.5, 0.0) + block_center);
        gs_out.base_texture_coord = id_xy_minus;
        gs_out.level_1_texture_coord = NO_TEXTURE;
        gs_out.color_modifier = color_modifier;
        EmitVertex();
        EndPrimitive();
    }

    //FACE XY+
    if ((block_faces & (1 << 4)) > 0) {
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, 0.5, 0.5, 0.0) + block_center);
        gs_out.base_texture_coord = id_xy_plus;
        gs_out.level_1_texture_coord = NO_TEXTURE;
        gs_out.color_modifier = color_modifier;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, 0.5, 0.5, 0.0) + block_center);
        gs_out.base_texture_coord = id_xy_plus + UP_RIGHT_OFF;
        gs_out.level_1_texture_coord = NO_TEXTURE;
        gs_out.color_modifier = color_modifier;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, -0.5, 0.5, 0.0) + block_center);
        gs_out.base_texture_coord = id_xy_plus + DOWN_LEFT_OFF;
        gs_out.level_1_texture_coord = NO_TEXTURE;
        gs_out.color_modifier = color_modifier;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, -0.5, 0.5, 0.0) + block_center);
        gs_out.base_texture_coord = id_xy_plus + DOWN_RIGHT_OFF;
        gs_out.level_1_texture_coord = NO_TEXTURE;
        gs_out.color_modifier = color_modifier;
        EmitVertex();
        EndPrimitive();
    }

    //FACE XZ-
    if ((block_faces & (1 << 1)) > 0) {
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, -0.5, 0.5, 0.0) + block_center);
        gs_out.base_texture_coord = id_xz_minus + DOWN_LEFT_OFF;
        gs_out.level_1_texture_coord = NO_TEXTURE;
        gs_out.color_modifier = color_modifier;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, -0.5, 0.5, 0.0) + block_center);
        gs_out.base_texture_coord = id_xz_minus;
        gs_out.level_1_texture_coord = NO_TEXTURE;
        gs_out.color_modifier = color_modifier;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, -0.5, -0.5, 0.0) + block_center);
        gs_out.base_texture_coord = id_xz_minus + DOWN_RIGHT_OFF;
        gs_out.level_1_texture_coord = NO_TEXTURE;
        gs_out.color_modifier = color_modifier;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, -0.5, -0.5, 0.0) + block_center);
        gs_out.base_texture_coord = id_xz_minus + UP_RIGHT_OFF;
        gs_out.level_1_texture_coord = NO_TEXTURE;
        gs_out.color_modifier = color_modifier;
        EmitVertex();
        EndPrimitive();
    }

    //FACE XZ+
    if ((block_faces & 1) > 0) {
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, 0.5, -0.5, 0.0) + block_center);
        gs_out.base_texture_coord = id_xz_plus + UP_RIGHT_OFF;
        gs_out.level_1_texture_coord = NO_TEXTURE;
        gs_out.color_modifier = color_modifier;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, 0.5, -0.5, 0.0) + block_center);
        gs_out.base_texture_coord = id_xz_plus + DOWN_RIGHT_OFF;
        gs_out.level_1_texture_coord = NO_TEXTURE;
        gs_out.color_modifier = color_modifier;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, 0.5, 0.5, 0.0) + block_center);
        gs_out.base_texture_coord = id_xz_plus;
        gs_out.level_1_texture_coord = NO_TEXTURE;
        gs_out.color_modifier = color_modifier;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, 0.5, 0.5, 0.0) + block_center);
        gs_out.base_texture_coord = id_xz_plus + DOWN_LEFT_OFF;
        gs_out.level_1_texture_coord = NO_TEXTURE;
        gs_out.color_modifier = color_modifier;
        EmitVertex();
        EndPrimitive();
    }

    //FACE YZ-
    if ((block_faces & (1 << 3)) > 0) {
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, -0.5, -0.5, 0.0) + block_center);
        gs_out.base_texture_coord = id_yz_minus + DOWN_LEFT_OFF;
        gs_out.level_1_texture_coord = NO_TEXTURE;
        gs_out.color_modifier = color_modifier;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, 0.5, -0.5, 0.0) + block_center);
        gs_out.base_texture_coord = id_yz_minus;
        gs_out.level_1_texture_coord = NO_TEXTURE;
        gs_out.color_modifier = color_modifier;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, -0.5, 0.5, 0.0) + block_center);
        gs_out.base_texture_coord = id_yz_minus + DOWN_RIGHT_OFF;
        gs_out.level_1_texture_coord = NO_TEXTURE;
        gs_out.color_modifier = color_modifier;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, 0.5, 0.5, 0.0) + block_center);
        gs_out.base_texture_coord = id_yz_minus + UP_RIGHT_OFF;
        gs_out.level_1_texture_coord = NO_TEXTURE;
        gs_out.color_modifier = color_modifier;
        EmitVertex();
        EndPrimitive();
    }

    //FACE YZ+
    if ((block_faces & (1 << 2)) > 0) {
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, -0.5, 0.5, 0.0) + block_center);
        gs_out.base_texture_coord = id_yz_plus + DOWN_LEFT_OFF;
        gs_out.level_1_texture_coord = NO_TEXTURE;
        gs_out.color_modifier = color_modifier;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, 0.5, 0.5, 0.0) + block_center);
        gs_out.base_texture_coord = id_yz_plus;
        gs_out.level_1_texture_coord = NO_TEXTURE;
        gs_out.color_modifier = color_modifier;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, -0.5, -0.5, 0.0) + block_center);
        gs_out.base_texture_coord = id_yz_plus +  DOWN_RIGHT_OFF;
        gs_out.level_1_texture_coord = NO_TEXTURE;
        gs_out.color_modifier = color_modifier;
        EmitVertex();
        gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, 0.5, -0.5, 0.0) + block_center);
        gs_out.base_texture_coord = id_yz_plus + UP_RIGHT_OFF;
        gs_out.level_1_texture_coord = NO_TEXTURE;
        gs_out.color_modifier = color_modifier;
        EmitVertex();
        EndPrimitive();
    }
}

// Array for texture coordinate
const vec2 tex_xy_plus[] = vec2[TOTAL_TEX](STONE, GRASS_TOP, DIRT, COBBLESTONE, PLANK, BEDROCK, SAND, WATER, SNOW, SNOW_GRASS,
WOOD_SIDE, FOLIAGE, TNT_SIDE, BRICKS, GLASS, ICE);
const vec2 tex_xy_minus[] = vec2[TOTAL_TEX](STONE, GRASS_TOP, DIRT, COBBLESTONE, PLANK, BEDROCK, SAND, WATER, SNOW, SNOW_GRASS,
WOOD_SIDE, FOLIAGE, TNT_SIDE, BRICKS, GLASS, ICE);
const vec2 tex_xz_plus[] = vec2[TOTAL_TEX](STONE, GRASS_TOP, DIRT, COBBLESTONE, PLANK, BEDROCK, SAND, WATER, SNOW, SNOW_GRASS,
WOOD_TOP_BOT, FOLIAGE, TNT_TOP, BRICKS, GLASS, ICE);
const vec2 tex_xz_minus[] = vec2[TOTAL_TEX](STONE, GRASS_TOP, DIRT, COBBLESTONE, PLANK, BEDROCK, SAND, WATER, SNOW, SNOW_GRASS,
WOOD_TOP_BOT, FOLIAGE, TNT_BOT, BRICKS, GLASS, ICE);
const vec2 tex_yz_plus[] = vec2[TOTAL_TEX](STONE, GRASS_TOP, DIRT, COBBLESTONE, PLANK, BEDROCK, SAND, WATER, SNOW, SNOW_GRASS,
WOOD_SIDE, FOLIAGE, TNT_SIDE, BRICKS, GLASS, ICE);
const vec2 tex_yz_minus[] = vec2[TOTAL_TEX](STONE, GRASS_TOP, DIRT, COBBLESTONE, PLANK, BEDROCK, SAND, WATER, SNOW, SNOW_GRASS,
WOOD_SIDE, FOLIAGE, TNT_SIDE, BRICKS, GLASS, ICE);
vec4 tex_color_modifier[] = vec4[TOTAL_TEX](NO_COLOR_MOD, NO_COLOR_MOD, NO_COLOR_MOD, NO_COLOR_MOD, NO_COLOR_MOD, NO_COLOR_MOD,
NO_COLOR_MOD, NO_COLOR_MOD, NO_COLOR_MOD, NO_COLOR_MOD, NO_COLOR_MOD, uniform_vec_color_modifier, NO_COLOR_MOD, NO_COLOR_MOD, NO_COLOR_MOD, NO_COLOR_MOD);

void main()
{
    int type = gs_in[0].block_type;
    if (type == 2) {
        generate_grass_block(gs_in[0].block_position, gs_in[0].block_faces, uniform_vec_color_modifier);
    } else {
        type -= 1;
        generate_textured_block(gs_in[0].block_position, gs_in[0].block_faces, tex_xy_plus[type], tex_xy_plus[type],
        tex_xz_plus[type], tex_xz_minus[type], tex_yz_plus[type], tex_yz_minus[type], tex_color_modifier[type]);
    }
}