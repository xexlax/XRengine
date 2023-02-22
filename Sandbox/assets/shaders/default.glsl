#type vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aTangent;
layout(location = 3) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;


void main()
{
    FragPos = vec3(u_Transform * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(u_Transform))) * aNormal;
    TexCoords = aTexCoords;
    gl_Position = u_ViewProjection * vec4(FragPos, 1.0);
}


#type fragment
#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;


uniform vec3 viewPos;

struct DirectionalLight {
    vec3 direction;
    vec3 color;
};

struct PointLight {

    vec3 position;
    float intensity;
    vec3 color;
    int enable;

};

struct Material {
   
    bool enable_diffuseTex;
    bool enable_specularTex;
    bool enable_bumpTex;
    sampler2D diffuseTex;
    sampler2D specularTex;
    sampler2D bumpTex;
    float shininess;
};

uniform DirectionalLight d_light;
uniform PointLight p_light[3];

uniform Material material;

float light_ambient=0.2;
float light_diffuse=0.5;
float light_specular=1.0;

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 合并结果
    vec3 ambient  = light_ambient *light.color  * vec3(texture(material.diffuseTex, TexCoords));
    vec3 diffuse  = light_diffuse *light.color * diff * vec3(texture(material.diffuseTex, TexCoords));
    vec3 specular = light_specular *light.color * spec * vec3(texture(material.specularTex, TexCoords));
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 衰减
    float distance    = length(light.position - fragPos);
    float attenuation = light.intensity / (distance * distance) ;    
    // 合并结果
    vec3 ambient  = light_ambient *light.color * vec3(texture(material.diffuseTex, TexCoords));
    vec3 diffuse  = light_diffuse *light.color * diff * vec3(texture(material.diffuseTex, TexCoords));
    vec3 specular = light_specular *light.color * spec * vec3(texture(material.specularTex, TexCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

void main()
{
    

    // diffuse 
    vec3 norm = normalize(Normal);
    
    vec3 viewDir =  normalize(viewPos - FragPos);

    vec3 result = CalcDirLight(d_light,norm,viewDir);
    for(int i=0;i<3;i++){
        if(p_light[i].enable==1){
            result+=CalcPointLight(p_light[i],norm,FragPos,viewDir);
        }
    }
    FragColor = vec4(result, 1.0);
}