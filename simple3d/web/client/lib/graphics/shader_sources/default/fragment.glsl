#version 300 es
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
in float shininess;

out vec4 frag_color;

uniform vec3 view_pos;

uniform vec3 ambient_light;

uniform int directional_light_count;
#define DIRECTIONAL_LIGHTS 8
uniform DirectionalLight directional_light[DIRECTIONAL_LIGHTS];

// should I include normal & view_pos here?
vec3 calc_dir_light(DirectionalLight light, vec3 normal_, vec3 view_dir) {
  vec3 diffuse = light.diffuse * max(dot(normal_, -light.direction), 0.0) * diffuse_color;

  vec3 reflect_dir = reflect(light.direction, normal_);

  float specular_mult = pow(max(dot(view_dir, reflect_dir), 0.0), shininess);
  vec3 specular = specular_mult * light.specular * specular_color;
  return (diffuse + specular);
}

void main() {
  vec3 ambient = ambient_light * diffuse_color;

  vec3 cum_frag_color = vec3(0.0);
  cum_frag_color += ambient;

  vec3 view_dir = normalize(view_pos - pos);
  vec3 normal_ = normalize(normal);
  int directional_light_count_ = min(directional_light_count, DIRECTIONAL_LIGHTS);
  for (int i = 0; i < directional_light_count_; i++) {
    cum_frag_color += calc_dir_light(directional_light[i], normal_, view_dir);
  }

  frag_color = vec4(cum_frag_color, 1.0);
}