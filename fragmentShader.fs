#version 330 core

out vec4 FragColor;

struct Material {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};

struct PointLight {              //Point Light
    vec3 position;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;

    float Kc;
    float Kl;
    float Kq;

};

struct DirectionalLight {              //Directional Light
    vec3 direction;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

struct SpotLight {                    //Spot Light
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;

    float Kc;
    float Kl;
    float Kq;
};

uniform PointLight pointLight;
uniform DirectionalLight directionalLight;
uniform SpotLight spotLight;


uniform Material material;
uniform vec4 objectColor;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform bool lightingOn;
uniform bool dark;

in vec3 Normal;
in vec3 FragPos;

//Functions prototypes
vec4 CalcDirLight(Material material, DirectionalLight light, vec3 normal, vec3 fragPos);
vec4 CalcPointLight(Material material, PointLight light, vec3 normal, vec3 fragPos);
vec4 CalcSpotLight(Material material, SpotLight light, vec3 normal, vec3 fragPos);


void main()
{

    //Directional Light Calculation
    vec4 dirL = CalcDirLight(material, directionalLight, Normal, FragPos);


    //Point Light
    vec4 pointL = CalcPointLight(material, pointLight, Normal, FragPos);


    //Spot Light
    vec4 spotL = CalcSpotLight(material, spotLight, Normal, FragPos);


    //Aggregate all result

    vec4 result = dirL + pointL + spotL;

    if(!lightingOn)
    {
        result = material.ambient;
    }
    if(dark)
    {
        result = vec4(0.0f);
    }

    FragColor = result;
}

vec4 CalcDirLight(Material material, DirectionalLight light, vec3 normal, vec3 fragPos)
{
    vec4 ambient = light.ambient * material.ambient;
    vec3 norm = normalize(normal);

    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = light.diffuse * (diff * material.diffuse);

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec4 specular = light.specular * (spec * material.specular);

    return (ambient + diffuse + specular);
}

vec4 CalcPointLight(Material material, PointLight light, vec3 normal, vec3 fragPos)
{
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.Kc + light.Kl * distance + light.Kq * (distance * distance));

    vec4 ambient = light.ambient * material.ambient;
    ambient *= attenuation;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = light.diffuse * (diff * material.diffuse);
    diffuse *= attenuation;

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec4 specular = light.specular * (spec * material.specular);
    specular *= attenuation;

    return (ambient + diffuse + specular );
}

vec4 CalcSpotLight(Material material, SpotLight light, vec3 normal, vec3 fragPos)
{
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.Kc + light.Kl * distance + light.Kq * (distance * distance));

    vec4 ambient = light.ambient * material.ambient;
    ambient *= attenuation;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = light.diffuse * (diff * material.diffuse);
    diffuse *= attenuation;

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec4 specular = light.specular * (spec * material.specular);
    specular *= attenuation;

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    ambient *= intensity;
    diffuse *= intensity;
    specular *= intensity;

    return (ambient + diffuse + specular);
}