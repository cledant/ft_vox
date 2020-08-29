#version 410 core

uniform sampler2D uniform_tex_diffuse;
uniform vec4 uniform_vec_color_modifier;
uniform float uniform_log_z_fighting;

in GS_OUT {
    vec2 base_texture_coord;
    vec2 level_1_texture_coord;
    float depth_z;
    flat bool use_color_modifier;
    flat bool use_level_1_color_modifier;
    flat bool use_level_1_texture;
} fs_in;

out vec4 color;

#define NO_TEXTURE vec2(2.0)

// Constants for Z fighting
#define C  1.0
#define FAR 8000.0
#define OFFSET 1.0

void main()
{
    color = texture(uniform_tex_diffuse, fs_in.base_texture_coord);
    if (fs_in.use_color_modifier) {
        color *= uniform_vec_color_modifier;
    }
    if (fs_in.use_level_1_texture) {
        vec4 level_1_tex_color = texture(uniform_tex_diffuse, fs_in.level_1_texture_coord);
        if (level_1_tex_color.a < 0.5) {
            color = texture(uniform_tex_diffuse, fs_in.base_texture_coord);
        } else if (fs_in.use_level_1_color_modifier) {
            color = uniform_vec_color_modifier * level_1_tex_color;
        } else {
            color = level_1_tex_color;
        }
    }
    if (color.a < 0.5) {
        discard;
    }
    gl_FragDepth = log(fs_in.depth_z * C + OFFSET) * uniform_log_z_fighting;
}