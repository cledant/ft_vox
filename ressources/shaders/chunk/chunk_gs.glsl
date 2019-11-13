#version 410 core

layout (points) in;
layout (triangle_strip, max_vertices = 24) out;

uniform mat4 uniform_mat_perspec_view;

in VS_OUT {
    vec4 block_position;
    uint block;
} gs_in[];

out GS_OUT {
    vec3 color;
} gs_out;

const vec3 RED = vec3(1.0, 0.0, 0.0);
const vec3 GREEN = vec3(0.0, 1.0, 0.0);
const vec3 BLUE = vec3(0.0, 0.0, 1.0);
const vec3 YELLOW = vec3(1.0, 1.0, 0.0);
const vec3 MAGENTA = vec3(1.0, 0.0, 1.0);
const vec3 CYAN = vec3(0.0, 1.0, 1.0);

void generate_block(vec4 block_center)
{
    //FACE XY-
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, -0.5, -0.5, 0.0) + block_center);
    gs_out.color = RED;
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, -0.5, -0.5, 0.0) + block_center);
    gs_out.color = RED;
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, 0.5, -0.5, 0.0) + block_center);
    gs_out.color = RED;
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, 0.5, -0.5, 0.0) + block_center);
    gs_out.color = RED;
    EmitVertex();
    EndPrimitive();

    //FACE XY+
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, -0.5, 0.5, 0.0) + block_center);
    gs_out.color = GREEN;
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, -0.5, 0.5, 0.0) + block_center);
    gs_out.color = GREEN;
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, 0.5, 0.5, 0.0) + block_center);
    gs_out.color = GREEN;
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, 0.5, 0.5, 0.0) + block_center);
    gs_out.color = GREEN;
    EmitVertex();
    EndPrimitive();

    //FACE XZ-
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, -0.5, -0.5, 0.0) + block_center);
    gs_out.color = BLUE;
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, -0.5, -0.5, 0.0) + block_center);
    gs_out.color = BLUE;
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, -0.5, 0.5, 0.0) + block_center);
    gs_out.color = BLUE;
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, -0.5, 0.5, 0.0) + block_center);
    gs_out.color = BLUE;
    EmitVertex();
    EndPrimitive();

    //FACE XZ+
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, 0.5, -0.5, 0.0) + block_center);
    gs_out.color = YELLOW;
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, 0.5, -0.5, 0.0) + block_center);
    gs_out.color = YELLOW;
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, 0.5, 0.5, 0.0) + block_center);
    gs_out.color = YELLOW;
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, 0.5, 0.5, 0.0) + block_center);
    gs_out.color = YELLOW;
    EmitVertex();
    EndPrimitive();

    //FACE YZ-
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, -0.5, -0.5, 0.0) + block_center);
    gs_out.color = MAGENTA;
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, 0.5, -0.5, 0.0) + block_center);
    gs_out.color = MAGENTA;
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, -0.5, 0.5, 0.0) + block_center);
    gs_out.color = MAGENTA;
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, 0.5, 0.5, 0.0) + block_center);
    gs_out.color = MAGENTA;
    EmitVertex();
    EndPrimitive();

    //FACE YZ+
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, -0.5, -0.5, 0.0) + block_center);
    gs_out.color = CYAN;
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, 0.5, -0.5, 0.0) + block_center);
    gs_out.color = CYAN;
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, -0.5, 0.5, 0.0) + block_center);
    gs_out.color = CYAN;
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, 0.5, 0.5, 0.0) + block_center);
    gs_out.color = CYAN;
    EmitVertex();
    EndPrimitive();
}

void main()
{
    uint type = gs_in[0].block & 31;

    if (type > 0) {
        generate_block(gs_in[0].block_position);
    }
}