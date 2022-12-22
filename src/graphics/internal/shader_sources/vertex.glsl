#version 310 es
precision highp float;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in mat4 aModel;
layout (location = 7) in vec3 aColor;

out vec3 ourColor;

// uniform mat4 model_test;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * aModel * vec4(aPos, 1.0);
    ourColor = vec3(1.0, aTexCoords.xy);
}