#version 310 es
precision highp float;

in vec3 diffuse_color;
in vec3 specular_color;

out vec4 frag_color;

void main() {
    frag_color = vec4(diffuse_color, 1.0);
}