#version 410 core

uniform sampler2D uniform_tex_diffuse;
uniform vec4 uniform_vec_color_modifier;
uniform float uniform_log_z_fighting;

in GS_OUT {
    vec2 base_texture_coord;
    float depth_z;
} fs_in;

out vec4 color;

// Constants for Z fighting
#define C  1.0
#define FAR 8000.0
#define OFFSET 1.0

void main()
{
    color = texture(uniform_tex_diffuse, fs_in.base_texture_coord);
    if (color.a < 0.5) {
        discard;
    }
    gl_FragDepth = log(fs_in.depth_z * C + OFFSET) * uniform_log_z_fighting;
}
