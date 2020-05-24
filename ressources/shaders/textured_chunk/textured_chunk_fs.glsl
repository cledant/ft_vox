#version 410 core

uniform sampler2D   uniform_tex_diffuse;

in GS_OUT {
    vec2 texture_coord;
    vec4 color_modifier;
} fs_in;

out vec4 color;

void main()
{
    color = fs_in.color_modifier * texture(uniform_tex_diffuse, fs_in.texture_coord);
    if (color.a < 0.5) {
        discard;
    }
}