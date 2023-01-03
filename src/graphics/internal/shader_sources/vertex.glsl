#version 310 es
precision highp float;

layout (location = 0) in vec3 pos_in;
layout (location = 1) in vec3 normal_in;
layout (location = 2) in vec2 tex_coords_in;
layout (location = 3) in mat4 model_in;

layout (location = 7) in vec3 diffuse_color_in;
layout (location = 8) in vec3 specular_color_in;

out vec3 pos;
out vec3 normal;
out vec3 diffuse_color;
out vec3 specular_color;

uniform mat4 view;
uniform mat4 projection;

uniform bool diffuse_texture_enabled;
uniform bool specular_texture_enabled;

void main() {
    gl_Position = projection * view * model_in * vec4(pos_in, 1.0);

    pos = (model_in * vec4(pos_in, 1.0)).xyz;

    // FIXME: normal is not in model space
    normal = normal_in;

    diffuse_color = vec3(1.0);
    specular_color = vec3(1.0);
}