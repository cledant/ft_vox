#version 410 core

layout (points) in;
layout (triangle_strip, max_vertices = 24) out;

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
    gs_out.texture_coord = vec2(0.25, TWO_THIRD);
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(uniform_skybox_size, -uniform_skybox_size, -uniform_skybox_size, 0.0));
    gs_out.texture_coord = vec2(0.0, TWO_THIRD);
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-uniform_skybox_size, uniform_skybox_size, -uniform_skybox_size, 0.0));
    gs_out.texture_coord = vec2(0.25, ONE_THIRD);
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(uniform_skybox_size, uniform_skybox_size, -uniform_skybox_size, 0.0));
    gs_out.texture_coord = vec2(0.0, ONE_THIRD);
    EmitVertex();
    EndPrimitive();

    //FACE XY+
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-uniform_skybox_size, uniform_skybox_size, uniform_skybox_size, 0.0));
    gs_out.texture_coord = vec2(0.5, ONE_THIRD);
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(uniform_skybox_size, uniform_skybox_size, uniform_skybox_size, 0.0));
    gs_out.texture_coord = vec2(0.75, ONE_THIRD);
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-uniform_skybox_size, -uniform_skybox_size, uniform_skybox_size, 0.0));
    gs_out.texture_coord = vec2(0.5, TWO_THIRD);
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(uniform_skybox_size, -uniform_skybox_size, uniform_skybox_size, 0.0));
    gs_out.texture_coord = vec2(0.75, TWO_THIRD);
    EmitVertex();
    EndPrimitive();

    //FACE XZ-
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-uniform_skybox_size, -uniform_skybox_size, uniform_skybox_size, 0.0));
    gs_out.texture_coord = vec2(1.0, TWO_THIRD);
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(uniform_skybox_size, -uniform_skybox_size, uniform_skybox_size, 0.0));
    gs_out.texture_coord = vec2(0.75, TWO_THIRD);
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-uniform_skybox_size, -uniform_skybox_size, -uniform_skybox_size, 0.0));
    gs_out.texture_coord = vec2(1.0, 1.0);
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(uniform_skybox_size, -uniform_skybox_size, -uniform_skybox_size, 0.0));
    gs_out.texture_coord = vec2(0.75, 1.0);
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
    gs_out.texture_coord = vec2(0.75, 0.0);
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(uniform_skybox_size, uniform_skybox_size, uniform_skybox_size, 0.0));
    gs_out.texture_coord = vec2(0.75, ONE_THIRD);
    EmitVertex();
    EndPrimitive();

    //FACE YZ-
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-uniform_skybox_size, -uniform_skybox_size, -uniform_skybox_size, 0.0));
    gs_out.texture_coord = vec2(0.25, TWO_THIRD);
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-uniform_skybox_size, uniform_skybox_size, -uniform_skybox_size, 0.0));
    gs_out.texture_coord = vec2(0.25, ONE_THIRD);
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-uniform_skybox_size, -uniform_skybox_size, uniform_skybox_size, 0.0));
    gs_out.texture_coord = vec2(0.5, TWO_THIRD);
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(-uniform_skybox_size, uniform_skybox_size, uniform_skybox_size, 0.0));
    gs_out.texture_coord = vec2(0.5, ONE_THIRD);
    EmitVertex();
    EndPrimitive();

    //FACE YZ+
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(uniform_skybox_size, -uniform_skybox_size, uniform_skybox_size, 0.0));
    gs_out.texture_coord = vec2(0.75, TWO_THIRD);
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(uniform_skybox_size, uniform_skybox_size, uniform_skybox_size, 0.0));
    gs_out.texture_coord = vec2(0.75, ONE_THIRD);
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(uniform_skybox_size, -uniform_skybox_size, -uniform_skybox_size, 0.0));
    gs_out.texture_coord = vec2(1.0, TWO_THIRD);
    EmitVertex();
    gl_Position = uniform_mat_perspec_view * (gl_in[0].gl_Position + vec4(uniform_skybox_size, uniform_skybox_size, -uniform_skybox_size, 0.0));
    gs_out.texture_coord = vec2(1.0, ONE_THIRD);
    EmitVertex();
    EndPrimitive();
}