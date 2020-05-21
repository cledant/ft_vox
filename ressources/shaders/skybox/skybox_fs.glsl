#version 410 core

uniform sampler2D   uniform_tex_diffuse;

in GS_OUT {
    vec2 texture_coord;
} fs_in;

out vec4 color;

void main()
{
    color = vec4(1.0) * texture(uniform_tex_diffuse, fs_in.texture_coord);
}