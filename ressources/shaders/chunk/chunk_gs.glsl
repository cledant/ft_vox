#version 410 core

#define BLOCK_PER_LINE 16
#define LINE_PER_PLANE 16
#define PLANE_PER_CHUNK 256
#define BLOCK_PER_CHUNK BLOCK_PER_LINE * LINE_PER_PLANE * PLANE_PER_CHUNK

layout (points) in;
layout (triangle_strip, max_vertices = 24) out;

uniform mat4 uniform_mat_perspec_view;

out VS_OUT {
    uint chunk_block;
} gs_in;

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
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, -0.5, -0.5, 0.0));
    gs_out.color = MAGENTA;
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, 0.5, -0.5, 0.0));
    gs_out.color = MAGENTA;
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, -0.5, 0.5, 0.0));
    gs_out.color = MAGENTA;
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-0.5, 0.5, 0.5, 0.0));
    gs_out.color = MAGENTA;
    EmitVertex();
    EndPrimitive();

    //FACE YZ+
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, -0.5, -0.5, 0.0));
    gs_out.color = CYAN;
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, 0.5, -0.5, 0.0));
    gs_out.color = CYAN;
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, -0.5, 0.5, 0.0));
    gs_out.color = CYAN;
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(0.5, 0.5, 0.5, 0.0));
    gs_out.color = CYAN;
    EmitVertex();
    EndPrimitive();
}

void main()
{
    int id_in_chunk = gl_InstanceID % BLOCK_PER_CHUNK;
    int id_in_plane = id_in_chunk % (BLOCK_PER_LINE * LINE_PER_PLANE);

    generate_block(vec4(id_in_plane % BLOCK_PER_LINE,
    id_in_plance / LINE_PER_PLANE,
    gl_InstanceID / PLANE_PER_CHUNK,
    0.0));
}