#version 410 core

layout (points) in;
layout (triangle_strip, max_vertices = 24) out;

// skybox.png texture has soft edges
// Adding or substracting a delta is necessary
const float DELTA = 0.001;
const float ONE_THIRD_PLUS_DELTA = (1.0 / 3.0) + DELTA;
const float TWO_THIRD_MINUS_DELTA = (2.0 / 3.0) - DELTA;
const float THREE_QUARTER_PLUS_DELTA = 0.75 + DELTA;
const float ONE_THIRD = (1.0 / 3.0);
const float TWO_THIRD = (2.0 / 3.0);

uniform mat4 uniform_mat_perspec_view;
uniform float uniform_skybox_size;

out GS_OUT {
    vec2 texture_coord;
} gs_out;

void main()
{
    //FACE XY-
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-uniform_skybox_size, -uniform_skybox_size, -uniform_skybox_size, 0.0));
    gs_out.texture_coord = vec2(0.25, TWO_THIRD_MINUS_DELTA);
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(uniform_skybox_size, -uniform_skybox_size, -uniform_skybox_size, 0.0));
    gs_out.texture_coord = vec2(0.0, TWO_THIRD_MINUS_DELTA);
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-uniform_skybox_size, uniform_skybox_size, -uniform_skybox_size, 0.0));
    gs_out.texture_coord = vec2(0.25, ONE_THIRD_PLUS_DELTA);
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(uniform_skybox_size, uniform_skybox_size, -uniform_skybox_size, 0.0));
    gs_out.texture_coord = vec2(0.0, ONE_THIRD_PLUS_DELTA);
    EmitVertex();
    EndPrimitive();

    //FACE XY+
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-uniform_skybox_size, uniform_skybox_size, uniform_skybox_size, 0.0));
    gs_out.texture_coord = vec2(0.5, ONE_THIRD_PLUS_DELTA);
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(uniform_skybox_size, uniform_skybox_size, uniform_skybox_size, 0.0));
    gs_out.texture_coord = vec2(THREE_QUARTER_PLUS_DELTA, ONE_THIRD_PLUS_DELTA);
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-uniform_skybox_size, -uniform_skybox_size, uniform_skybox_size, 0.0));
    gs_out.texture_coord = vec2(0.5, TWO_THIRD_MINUS_DELTA);
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(uniform_skybox_size, -uniform_skybox_size, uniform_skybox_size, 0.0));
    gs_out.texture_coord = vec2(THREE_QUARTER_PLUS_DELTA, TWO_THIRD_MINUS_DELTA);
    EmitVertex();
    EndPrimitive();

    //FACE XZ-
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-uniform_skybox_size, -uniform_skybox_size, uniform_skybox_size, 0.0));
    gs_out.texture_coord = vec2(THREE_QUARTER_PLUS_DELTA, 1.0);
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(uniform_skybox_size, -uniform_skybox_size, uniform_skybox_size, 0.0));
    gs_out.texture_coord = vec2(THREE_QUARTER_PLUS_DELTA, TWO_THIRD);
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-uniform_skybox_size, -uniform_skybox_size, -uniform_skybox_size, 0.0));
    gs_out.texture_coord = vec2(1.0, 1.0);
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(uniform_skybox_size, -uniform_skybox_size, -uniform_skybox_size, 0.0));
    gs_out.texture_coord = vec2(1.0, TWO_THIRD);
    EmitVertex();
    EndPrimitive();

    //FACE XZ+
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-uniform_skybox_size, uniform_skybox_size, -uniform_skybox_size, 0.0));
    gs_out.texture_coord = vec2(1.0, 0.0);
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(uniform_skybox_size, uniform_skybox_size, -uniform_skybox_size, 0.0));
    gs_out.texture_coord = vec2(1.0, ONE_THIRD);
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-uniform_skybox_size, uniform_skybox_size, uniform_skybox_size, 0.0));
    gs_out.texture_coord = vec2(THREE_QUARTER_PLUS_DELTA, 0.0);
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(uniform_skybox_size, uniform_skybox_size, uniform_skybox_size, 0.0));
    gs_out.texture_coord = vec2(THREE_QUARTER_PLUS_DELTA, ONE_THIRD);
    EmitVertex();
    EndPrimitive();

    //FACE YZ-
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-uniform_skybox_size, -uniform_skybox_size, -uniform_skybox_size, 0.0));
    gs_out.texture_coord = vec2(0.25, TWO_THIRD_MINUS_DELTA);
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-uniform_skybox_size, uniform_skybox_size, -uniform_skybox_size, 0.0));
    gs_out.texture_coord = vec2(0.25, ONE_THIRD_PLUS_DELTA);
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-uniform_skybox_size, -uniform_skybox_size, uniform_skybox_size, 0.0));
    gs_out.texture_coord = vec2(0.5, TWO_THIRD_MINUS_DELTA);
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-uniform_skybox_size, uniform_skybox_size, uniform_skybox_size, 0.0));
    gs_out.texture_coord = vec2(0.5, ONE_THIRD_PLUS_DELTA);
    EmitVertex();
    EndPrimitive();

    //FACE YZ+
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(uniform_skybox_size, -uniform_skybox_size, uniform_skybox_size, 0.0));
    gs_out.texture_coord = vec2(THREE_QUARTER_PLUS_DELTA, TWO_THIRD_MINUS_DELTA);
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(uniform_skybox_size, uniform_skybox_size, uniform_skybox_size, 0.0));
    gs_out.texture_coord = vec2(THREE_QUARTER_PLUS_DELTA, ONE_THIRD_PLUS_DELTA);
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(uniform_skybox_size, -uniform_skybox_size, -uniform_skybox_size, 0.0));
    gs_out.texture_coord = vec2(1.0, TWO_THIRD_MINUS_DELTA);
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(uniform_skybox_size, uniform_skybox_size, -uniform_skybox_size, 0.0));
    gs_out.texture_coord = vec2(1.0, ONE_THIRD_PLUS_DELTA);
    EmitVertex();
    EndPrimitive();
}