#version 310 es
precision highp float;

struct DirectionalLight {
    vec3 direction;
    vec3 diffuse;
    vec3 specular;
};

in vec3 pos;
in vec3 normal;
in vec3 diffuse_color;
in vec3 specular_color;

out vec4 frag_color;

uniform vec3 view_pos;
// uniform vec3 ambient_light;
// #define DIRECTIONAL_LIGHTS 1
// uniform DirectionalLight directional_light[DIRECTIONAL_LIGHTS]

void main() {
    vec3 ambient_light = vec3(1.0) * 0.3;
    DirectionalLight directional_light = DirectionalLight(
        normalize(vec3(0.2, 0.4, -1.0)),
        vec3(1.0) * 0.5,
        vec3(1.0) * 0.3
    );

    vec3 ambient = ambient_light * diffuse_color;
    vec3 diffuse = directional_light.diffuse * max(dot(normal, -directional_light.direction), 0.0) * diffuse_color;

    vec3 reflect_dir = reflect(directional_light.direction, normal);
    vec3 view_dir = normalize(view_pos - pos);
    float specular_mult = pow(max(dot(view_dir, reflect_dir), 0.0), 16.0);
    vec3 specular = specular_mult * directional_light.specular * specular_color;
    // vec3 specular = pow(max(dot(view_dir, reflect_dir), 0.0), 16) * 0.5 * directional_light.specular * specular_color;

    frag_color = vec4(ambient + diffuse + specular, 1.0);
}