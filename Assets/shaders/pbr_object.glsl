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
out vec4 FragPosLightSpace;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
uniform mat4 lightSpaceMatrix;


void main()
{
    FragPos = vec3(u_Transform * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(u_Transform))) * aNormal;
    TexCoords = aTexCoords;
    Tangent = aTangent;
    gl_Position = u_ViewProjection * vec4(FragPos, 1.0);
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
}


#type fragment
#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
in vec3 Tangent;
in vec4 FragPosLightSpace;

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
uniform sampler2D shadowMap;

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

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}  

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}


float ShadowCalculation(vec4 fragPosLightSpace ,float bias)
{
    
    // 执行透视除法
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // 变换到[0,1]的范围
    projCoords = projCoords * 0.5 + 0.5;
    // 取得最近点的深度(使用[0,1]范围下的fragPosLight当坐标)
    
    // 取得当前片段在光源视角下的深度
    float currentDepth = projCoords.z;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);

    float shadow = 0.0;
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += (currentDepth - bias > pcfDepth ? 1.0 : 0.0)*4/(1<<(abs(x)+abs(y)));        
        }    
    }
    shadow /= 16.0;
    // 检查当前片段是否在阴影中
   
   // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    if(projCoords.z > 1.0)
        shadow = 0.0;
    //if(shadow>1) return 1.0;
    return shadow;
}

void main()
{
    

    // diffuse 
    
    vec3 N = GetNormal();
    vec3 V =  normalize(viewPos - FragPos);
    vec3 R = reflect(-V, N);
    vec3 albedo=GetAlbedo();
    float bias = 0.005;
    float shadow = ShadowCalculation(FragPosLightSpace,bias);  
    vec3 Lo = vec3(0.0);

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, material.metallic);

    //vec3 result = CalcDirLight(d_light,N,V);
    {
            vec3 L = normalize(-d_light.direction);
            vec3 H = normalize(V + L);

        
            vec3 radiance     = d_light.color * d_light.intensity ; 
    
            
            vec3 F  = fresnelSchlick(max(dot(H, V), 0.0), F0);

            float NDF = DistributionGGX(N, H, material.roughness);       
            float G   = GeometrySmith(N, V, L, material.roughness); 

            vec3 nominator    = NDF * G * F;
            float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; 
            vec3 specular     = nominator / denominator; 

            vec3 kS = F;
            vec3 kD = vec3(1.0) - kS;

            kD *= 1.0 - material.metallic;     

            float NdotL = max(dot(N, L), 0.0);        
            Lo += (kD * albedo / PI + specular) * radiance * NdotL;   
    }
    
    
    for(int i=0;i<3;i++){
        if(p_light[i].enable==1){
            
            vec3 L = normalize(p_light[i].position - FragPos);
            vec3 H = normalize(V + L);

            float distance    = length(p_light[i].position - FragPos);
            float attenuation = 1.0 / (distance * distance);
            vec3 radiance     = p_light[i].color * attenuation *p_light[i].intensity; 
    
            vec3 F  = fresnelSchlick(max(dot(H, V), 0.0), F0);

            float NDF = DistributionGGX(N, H, material.roughness);       
            float G   = GeometrySmith(N, V, L, material.roughness); 

            vec3 nominator    = NDF * G * F;
            float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; 
            vec3 specular     = nominator / denominator; 

            vec3 kS = F;
            vec3 kD = vec3(1.0) - kS;

            kD *= 1.0 - material.metallic;     


            float NdotL = max(dot(N, L), 0.0);        
            Lo += (kD * albedo / PI + specular) * radiance * NdotL;     

        }
    }
    vec3 ambient = vec3(0.03) * albedo ;
    vec3 color = ambient + (1-shadow)*Lo;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));  
    FragColor = vec4(color, 1.0);
}