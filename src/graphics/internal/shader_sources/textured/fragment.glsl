#version 310 es
precision highp float;

struct DirectionalLight {
    vec3 direction;
    vec3 diffuse;
    vec3 specular;
};

in vec3 pos;
in vec3 normal;
in vec2 tex_coord;
// in float shininess;

out vec4 frag_color;

uniform vec3 view_pos;

uniform vec3 ambient_light;

uniform int directional_light_count;
#define DIRECTIONAL_LIGHTS 8
uniform DirectionalLight directional_light[DIRECTIONAL_LIGHTS];

struct Material {
  sampler2D texture_diffuse_1;
  sampler2D texture_diffuse_2;
  sampler2D texture_diffuse_3;
  sampler2D texture_diffuse_4;

  sampler2D texture_specular_1;
  sampler2D texture_specular_2;
  sampler2D texture_specular_3;
  sampler2D texture_specular_4;
};

uniform Material material;

// should I include normal & view_pos here?
vec3 calc_dir_light(DirectionalLight light, vec3 normal_, vec3 view_dir) {
  const float kDefaultShininess = 16.0;

  vec3 diffuse = light.diffuse * max(dot(normal_, -light.direction), 0.0)
      * vec3(texture(material.texture_diffuse_1, tex_coord));

  vec3 reflect_dir = reflect(light.direction, normal_);

  float specular_mult = pow(max(dot(view_dir, reflect_dir), 0.0), kDefaultShininess);
  vec3 specular = specular_mult * light.specular
      * vec3(texture(material.texture_specular_1, tex_coord));
  return (diffuse + specular);
}

void main() {
  vec3 ambient = ambient_light * vec3(texture(material.texture_diffuse_1, tex_coord));

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