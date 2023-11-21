#version 450
layout(location = 0) out vec4 FragColor;
//layout(location = 1) out int color1;

layout(location = 0) in vec3 FragPos;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec3 Tangent;
layout(location = 3) in vec2 TexCoords;
//layout(location = 4) in vec4 FragPosLightSpace;

struct PointLight {
	vec4 position;
	vec3 color;
	float intensity;
};

struct DirectionalLight{
    
	vec4 direction;
	vec3 color;
	float intensity;
};
layout(binding = 0) uniform GlobalUBO {
    mat4 view;
    mat4 proj;
    vec3 camPos;
} ubo;

layout(binding = 1) uniform LightingUBO{

    DirectionalLight d_light;
    PointLight p_light[4];
    int p_light_amount;
    

}lights;

const float PI = 3.14159265359;

layout(binding = 2) uniform sampler2D albedoMap;
layout(binding = 3) uniform sampler2D specularMap;
layout(binding = 4) uniform sampler2D normalMap;


layout(push_constant) uniform Push {
    mat4 model;
    int id;
} push;

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

vec3 GetAlbedo()
{

    return vec3(texture(albedoMap, TexCoords));

}

vec3 GetSpecular(){
    // if(material.enable_specularTex)
    //    return vec3(texture(material.specularTex, TexCoords));
    // else{
       return vec3(1.0,1.0,1.0);
    //}
}

vec3 GetNormal(){

    vec3 N = normalize(Normal);
    vec3 T = normalize(Tangent);
    vec3 B = normalize(cross(N,T));

    mat3 TBN = mat3(T,B,N);
    
    // if(material.enable_bumpTex){
    //    vec3 n= vec3(texture(material.bumpTex, TexCoords));
    //    n= normalize(n * 2.0 - 1.0);
    //    n= normalize(TBN * n);
    //    return n;
    // }


      return N;
    
}




void main()
{
    

    // diffuse 
    
    vec3 N = GetNormal();
    vec3 V =  normalize(ubo.camPos - FragPos);
    vec3 R = reflect(-V, N);
    vec3 albedo=GetAlbedo();
    float bias = 0.005;
    vec3 Lo = vec3(0.0);
    float shadow =0.0;

    vec3 F0 = vec3(0.04); 
    float metallic = 0.0;
    float roughness = 0.0;
    F0 = mix(F0, albedo, metallic);

    //vec3 result = CalcDirLight(d_light,N,V);
    {
            vec3 L = normalize(-lights.d_light.direction.xyz);
            vec3 H = normalize(V + L);
            vec3 radiance     = lights.d_light.color * lights.d_light.intensity ; 
    
            vec3 F  = fresnelSchlick(max(dot(H, V), 0.0), F0);

            float NDF = DistributionGGX(N, H, roughness);       
            float G   = GeometrySmith(N, V, L, roughness); 

            vec3 nominator    = NDF * G * F;
            float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; 
            vec3 specular     = nominator / denominator; 

            vec3 kS = F;
            vec3 kD = vec3(1.0) - kS;

            kD *= 1.0 - metallic;     

            float NdotL = max(dot(N, L), 0.0);        
            Lo += (kD * albedo / PI + specular) * radiance * NdotL;   
    }
    
    
    for(int i=0;i<lights.p_light_amount;i++){
        
            
            vec3 L = normalize(lights.p_light[i].position.xyz - FragPos);
            vec3 H = normalize(V + L);

            float distance    = length(lights.p_light[i].position.xyz - FragPos);
            float attenuation = 1.0 / (distance * distance);
            vec3 radiance     = lights.p_light[i].color * attenuation *lights.p_light[i].intensity; 
    
            vec3 F  = fresnelSchlick(max(dot(H, V), 0.0), F0);

            float NDF = DistributionGGX(N, H, roughness);       
            float G   = GeometrySmith(N, V, L, roughness); 

            vec3 nominator    = NDF * G * F;
            float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; 
            vec3 specular     = nominator / denominator; 

            vec3 kS = F;
            vec3 kD = vec3(1.0) - kS;

            kD *= 1.0 - metallic;     


            float NdotL = max(dot(N, L), 0.0);        
            Lo += (kD * albedo / PI + specular) * radiance * NdotL;     

        
    }
    vec3 ambient = vec3(0.03) * albedo ;
    vec3 color = ambient + (1-shadow)*Lo;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));  
    FragColor = vec4(color, 1.0);
}