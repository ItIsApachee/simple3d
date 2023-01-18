#version 310 es
precision highp float;

layout (location = 0) in vec3 pos_in;
layout (location = 1) in vec3 normal_in;
layout (location = 2) in mat4 model_in;
// to fix normal scaling
// layout (location = 6) in mat4 model_inv_in;

layout (location = 10) in vec3 diffuse_color_in;
layout (location = 11) in vec3 specular_color_in;
layout (location = 12) in float shininess_in;

out vec3 pos;
out vec3 normal;
out vec3 diffuse_color;
out vec3 specular_color;
out float shininess;

uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model_in * vec4(pos_in, 1.0);

    pos = vec3(model_in * vec4(pos_in, 1.0));

    // FIXME: scaling breaks normals
    normal = vec3(model_in * vec4(normal_in, 0.0));

    diffuse_color = diffuse_color_in;
    specular_color = specular_color_in;
    shininess = shininess_in;
}