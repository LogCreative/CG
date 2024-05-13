#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform sampler2D floorTexture;

uniform vec3 ambient;

struct PointLight {
    vec3 position;

    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

#define NR_POINT_LIGHTS 2
uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform vec3 viewPos;

vec3 CalcPointLight(PointLight light, vec3 color, vec3 normal, vec3 fragPos, vec3 viewDir) {

    vec3 lightDir = normalize(light.position - fs_in.FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * color);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0); // assume shinesss is 8.0
    vec3 specular = light.specular * spec * vec3(0.3); // assuming bright white light color

    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    diffuse  *= attenuation;
    specular *= attenuation;

    vec3 result = diffuse + specular;
    return result;
}

void main()
{
    vec3 norm = normalize(fs_in.Normal);
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);

    vec3 color = texture(floorTexture, fs_in.TexCoords).rgb;
    vec3 result = ambient * color;
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], color, norm, fs_in.FragPos, viewDir);

    FragColor = vec4(result, 1.0);
}