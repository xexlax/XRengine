#type vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aTangent;
layout(location = 3) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec3 Tangent;
out vec2 TexCoords;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;


void main()
{
    FragPos = vec3(u_Transform * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(u_Transform))) * aNormal;
    TexCoords = aTexCoords;
    Tangent = aTangent;
    gl_Position = u_ViewProjection * vec4(FragPos, 1.0);
}


#type fragment
#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
in vec3 Tangent;

uniform vec3 viewPos;

struct DirectionalLight {
    vec3 direction;
    float intensity;
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
    float metallic;
    float roughness;
};

uniform DirectionalLight d_light;
uniform PointLight p_light[3];

uniform Material material;

float light_ambient=0.3;
float light_diffuse=0.6;
float light_specular=1.0;
const float PI = 3.14159265359;
vec3 GetAlbedo()
{
    if(material.enable_diffuseTex)
       return vec3(texture(material.diffuseTex, TexCoords));
    else{
       return vec3(1.0,1.0,1.0);
    }
}

vec3 GetSpecular(){
    if(material.enable_specularTex)
       return vec3(texture(material.specularTex, TexCoords));
    else{
       return vec3(1.0,1.0,1.0);
    }
}

vec3 GetNormal(){

    vec3 N = normalize(Normal);
    vec3 T = normalize(Tangent);
    vec3 B = normalize(cross(N,T));

    mat3 TBN = mat3(T,B,N);
    
    if(material.enable_bumpTex){
       vec3 n= vec3(texture(material.bumpTex, TexCoords));
       n= normalize(n * 2.0 - 1.0);
       n= normalize(TBN * n);
       return n;
    }


      return N;
    

    
}
// BlinnPhong
vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    // 合并结果
    vec3 ambient  = light_ambient *light.color  * GetAlbedo();
    vec3 diffuse  = light_diffuse *light.color * diff * GetAlbedo();
    vec3 specular = light_specular *light.color * spec * GetSpecular();
    return (ambient + diffuse + specular)*light.intensity;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    // 衰减
    float distance    = length(light.position - fragPos);
    float attenuation = light.intensity / (distance * distance) ;    
    // 合并结果
    vec3 ambient  = light_ambient *light.color * GetAlbedo();
    vec3 diffuse  = light_diffuse *light.color * diff * GetAlbedo();
    vec3 specular = light_specular *light.color * spec * GetSpecular();
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}


void main()
{
    

    // diffuse 
    
    vec3 N = GetNormal();
    vec3 V =  normalize(viewPos - FragPos);
    vec3 R = reflect(-V, N);
    

   
    vec3 color = CalcDirLight(d_light,N,V);

    for(int i=0;i<3;i++){
        if(p_light[i].enable==1){
            color+=CalcPointLight(p_light[i],N,FragPos,V);
        }
    }
    
    FragColor = vec4(color, 1.0);
}