#version 330 core
out vec4 FragColor;

struct Material {
  sampler2D diffuse;
  sampler2D specular;
  float shininess;
};

struct DirLight {
  vec3 direction;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct PointLight {
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float constant;
  float linear;
  float quadratic;
};

struct SpotLight {
  vec3 position;
  vec3 direction;
  float cutOff;
  float outerCutOff;
  float constant;
  float linear;
  float quadratic;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

#define NR_POINT_LIGHTS 4

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
in vec3 LightPos;

uniform DirLight dirLight;
uniform PointLight pointLight[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;

// light functions
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 fragPos);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos);

void main() {
  vec3 norm = normalize(Normal);

  vec3 result;
  result = CalcDirLight(dirLight, norm, FragPos);
  for (int i = 0; i < NR_POINT_LIGHTS; i++) {
    result += CalcPointLight(pointLight[i], norm, FragPos);
  }
  result += CalcSpotLight(spotLight, norm, FragPos);
	FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 fragPos)
{
  vec3 diffTexRgb = texture(material.diffuse, TexCoords).rgb;
  vec3 ambient = light.ambient * diffTexRgb;

  vec3 lightDir = normalize(-light.direction);
  float diff = max(dot(normal, lightDir), 0.0);
  vec3 diffuse = light.diffuse * diff * diffTexRgb;

  vec3 viewDir = normalize(-fragPos);
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  vec3 specular = light.specular * spec * diffTexRgb;

  return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos)
{
  vec3 diffTexRgb = texture(material.diffuse, TexCoords).rgb;
  vec3 ambient = light.ambient * diffTexRgb;

  vec3 lightDir = normalize(light.position - fragPos);
  float diff = max(dot(normal, lightDir), 0.0);
  vec3 diffuse = light.diffuse * diff * diffTexRgb;

  vec3 viewDir = normalize(-fragPos);
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  vec3 specular = light.specular * spec * diffTexRgb;

  // attenuation
  float distance = length(light.position - fragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

  return (ambient + diffuse + specular) * attenuation;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos)
{
  vec3 diffTexRgb = texture(material.diffuse, TexCoords).rgb;
  vec3 ambient = light.ambient * diffTexRgb;

  vec3 lightDir = normalize(light.position - fragPos);
  float diff = max(dot(normal, lightDir), 0.0);
  vec3 diffuse = light.diffuse * diff * diffTexRgb;

  vec3 viewDir = normalize(-fragPos);
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  vec3 specular = light.specular * spec * diffTexRgb;

  // attenuation
  float distance = length(light.position - fragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

  // spot light intensity
  float theta = dot(lightDir, normalize(-light.direction));
  float epsilon = light.cutOff - light.outerCutOff;
  float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

  return (ambient + diffuse + specular) * attenuation * intensity;
}