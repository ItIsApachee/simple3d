#version 310 es
precision highp float;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 ourColor;

uniform mat4 model;

void main() {
    gl_Position = model * vec4(aPos, 1.0);
    // ourColor = vec3(1.0, 0.5+0.5*(0.1*aNormal.x + 0.2*aNormal.y + 0.4*aNormal.z), 0.0);
    ourColor = vec3(1.0, aTexCoords.xy);
}