#version 410 core

uniform sampler2D   uniform_tex_diffuse;

in GS_OUT {
    vec2 base_texture_coord;
    vec2 level_1_texture_coord;
    vec4 color_modifier;
} fs_in;

out vec4 color;

#define NO_TEXTURE vec2(2.0)

void main()
{
    if (fs_in.level_1_texture_coord == NO_TEXTURE) {
        color = fs_in.color_modifier * texture(uniform_tex_diffuse, fs_in.base_texture_coord);
        if (color.a < 0.5) {
            discard;
        }
        return;
    }
    vec4 level_1_tex_color = texture(uniform_tex_diffuse, fs_in.level_1_texture_coord);
    if (level_1_tex_color.a < 0.5) {
        color = texture(uniform_tex_diffuse, fs_in.base_texture_coord);
    } else {
        color = fs_in.color_modifier * level_1_tex_color;
    }
    if (color.a < 0.5) {
        discard;
    }
}