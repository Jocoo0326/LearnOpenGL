#version 330 core
struct Material {
  sampler2D diffuse;
  sampler2D specular;
  float shininess;
};
struct Light {
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform Material material;
uniform Light light;

out vec4 FragColor;

void main() {
  vec3 diffTexRgb = texture(material.diffuse, TexCoords).rgb; 
  vec3 ambient = light.ambient * diffTexRgb;

  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(light.position - FragPos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = light.diffuse * (diff * diffTexRgb);

  vec3 viewDir = normalize(-FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  vec3 specular = light.specular * (spec * texture(material.specular, TexCoords).rgb);

  vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}