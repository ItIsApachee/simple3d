#version 300 es
precision lowp float;

#define INPUT_ARRAY_AT(ARR0, ARR1, ARR2, index) \
    (index == 0) \
    ? ARR0 \
    : ((index == 1) \
    ? ARR1 \
    : ARR2)

layout (location = 0) in mat4 model_in;
layout (location = 4) in vec3 pos_0_in;
layout (location = 5) in vec3 pos_1_in;
layout (location = 6) in vec3 pos_2_in;
layout (location = 7) in vec3 diffuse_color_0_in;
layout (location = 8) in vec3 diffuse_color_1_in;
layout (location = 9) in vec3 diffuse_color_2_in;
layout (location = 10) in vec3 specular_color_0_in;
layout (location = 11) in vec3 specular_color_1_in;
layout (location = 12) in vec3 specular_color_2_in;
// i-th component is shininess for i-th vertex.
layout (location = 13) in vec3 shininess_in;
layout (location = 14) in vec3 normal_in;

out vec3 pos;
out vec3 normal;
out vec3 diffuse_color;
out vec3 specular_color;
out float shininess;

uniform mat4 view;
uniform mat4 projection;

void main() {
    vec3 pos_in = INPUT_ARRAY_AT(pos_0_in, pos_1_in, pos_2_in, gl_VertexID);
    vec3 diffuse_color_in = INPUT_ARRAY_AT(diffuse_color_0_in, diffuse_color_1_in, diffuse_color_2_in, gl_VertexID);
    vec3 specular_color_in = INPUT_ARRAY_AT(specular_color_0_in, specular_color_1_in, specular_color_2_in, gl_VertexID);

    gl_Position = projection * view * model_in * vec4(pos_in, 1.0);

    pos = vec3(model_in * vec4(pos_in, 1.0));

    // FIXME: scaling breaks normals
    normal = vec3(model_in * vec4(normal_in, 0.0));

    diffuse_color = diffuse_color_in;
    specular_color = specular_color_in;
    shininess = shininess_in[gl_VertexID];
}