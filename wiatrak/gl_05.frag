#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
  
out vec4 color;

uniform sampler2D Texture0;
//uniform sampler2D Texture1;

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	float constant;
    float linear;
    float quadratic;
};

uniform vec3 viewPos;

uniform int pointLightsQuantity;
uniform Light pointLights[100];

vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos); 
	
	vec3 result = vec3(0.0f);
	for(int i = 0; i < pointLightsQuantity; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);   
      
    color = vec4(result, 1.0);
}

vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
    
    // attenuation
	float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    // combine results
    vec3 ambient = light.ambient * texture(Texture0, TexCoord).rgb;
    vec3 diffuse = light.diffuse * diff * texture(Texture0, TexCoord).rgb;  
    vec3 specular = light.specular * spec;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}
