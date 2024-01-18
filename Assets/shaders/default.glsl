#type vertex
#version 450
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aTangent;
layout(location = 3) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec3 Tangent;
out vec2 TexCoords;
out vec4 FragPosLightSpace;

out vec4 LastFramePos;

out vec3 VSPos;
out vec3 VSNorm;
out vec3 ONorm;

uniform int u_Frame;
uniform bool u_TAA;
uniform float u_ScreenWidth,u_ScreenHeight;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_MVPLastFrame;
uniform mat4 u_Transform;
uniform mat4 lightSpaceMatrix;

const vec2 Halton_2_3[8] =
{
    vec2(0.0f, -1.0f / 3.0f),
    vec2(-1.0f / 2.0f, 1.0f / 3.0f),
    vec2(1.0f / 2.0f, -7.0f / 9.0f),
    vec2(-3.0f / 4.0f, -1.0f / 9.0f),
    vec2(1.0f / 4.0f, 5.0f / 9.0f),
    vec2(-1.0f / 4.0f, -5.0f / 9.0f),
    vec2(3.0f / 4.0f, 1.0f / 9.0f),
    vec2(-7.0f / 8.0f, 7.0f / 9.0f)
};


void main()
{
    int offsetIdx = u_Frame;
    float deltaWidth = 1.0f/u_ScreenWidth;
    float deltaHeight = 1.0f/u_ScreenHeight;

    vec2 jitter =  vec2(0,0);
    if(u_Frame >=0 && u_TAA)
    jitter= vec2(
        Halton_2_3[offsetIdx].x * deltaWidth,
        Halton_2_3[offsetIdx].y * deltaHeight
    );
    mat4 jitterMat = u_Projection;
    jitterMat[2][0] += jitter.x;
    jitterMat[2][1] += jitter.y;

    FragPos = vec3(u_Transform * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(u_Transform))) * aNormal;
    TexCoords = aTexCoords;
    Tangent = aTangent;

    
    gl_Position = jitterMat *u_View* vec4(FragPos, 1.0);

    LastFramePos = u_MVPLastFrame* vec4(aPos, 1.0) - gl_Position;

    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);

    VSPos = vec3(u_View*u_Transform * vec4(aPos, 1.0));
    VSNorm = mat3(transpose(inverse(u_View* u_Transform))) * aNormal;
    ONorm = mat3(u_Transform) * aNormal;

}


#type fragment
#version 450
layout(location = 0) out vec4 FragColor;
layout(location = 1) out int color1;

layout(location = 2) out vec4 ViewSpacePos;
layout(location = 3) out vec4 ViewSpaceNormal;
layout(location = 4) out vec4 OutNormal;
layout(location = 5) out vec4 OutMaterial;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
in vec3 Tangent;
in vec4 FragPosLightSpace;
in vec4 LastFramePos;

in vec3 VSPos;
in vec3 VSNorm;
in vec3 ONorm;

uniform vec3 viewPos;


#define N_SAMPLE 16
const vec2 poissonDisk[16] = {
    vec2( -0.94201624, -0.39906216 ),
    vec2( 0.94558609, -0.76890725 ),
    vec2( -0.094184101, -0.92938870 ),
    vec2( 0.34495938, 0.29387760 ),
    vec2( -0.91588581, 0.45771432 ),
    vec2( -0.81544232, -0.87912464 ),
    vec2( -0.38277543, 0.27676845 ),
    vec2( 0.97484398, 0.75648379 ),
    vec2( 0.44323325, -0.97511554 ),
    vec2( 0.53742981, -0.47373420 ),
    vec2( -0.26496911, -0.41893023 ),
    vec2( 0.79197514, 0.19090188 ),
    vec2( -0.24188840, 0.99706507 ),
    vec2( -0.81409955, 0.91437590 ),
    vec2( 0.19984126, 0.78641367 ),
    vec2( 0.14383161, -0.14100790 )
};




struct DirectionalLight {
    vec3 direction;
    float intensity;
    vec3 color;
    bool shadowCasting;
};

struct PointLight {

    vec3 position;
    float intensity;
    vec3 color;
    int enable;
    bool shadowCasting;

};

struct Material {
    vec3 baseColor;

    vec3 ambient;
    vec3 diffise;
    vec3 specular;
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
uniform PointLight p_light[10];
uniform int p_light_amount;

uniform Material material;
uniform sampler2D shadowMap;

uniform bool Shadow_On;
uniform int ObjID;

float near_plane=1;
float far_plane=1000;
float light_ambient=0.3;
float light_diffuse=0.6;
float light_specular=1.0;
const float PI = 3.14159265359;


vec3 GetAlbedo()
{
    vec3 albedo;
    if(material.enable_diffuseTex)
       albedo= vec3(texture(material.diffuseTex, TexCoords)) * material.baseColor;
    else{
       albedo= material.baseColor;
    }
    return albedo;
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



// BlinnPhong
vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir, float shadow)
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
    return (ambient +(1-shadow)*(diffuse + specular))*light.intensity;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir,float shadow)
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
    return (ambient + (1-shadow)*(diffuse + specular));
}

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}



float ShadowCalculation(vec4 fragPosLightSpace ,float bias)
{
    if(Shadow_On == false) return 0.0;
    // 执行透视除法
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // 变换到[0,1]的范围
    projCoords = projCoords * 0.5 + 0.5;
    // 取得最近点的深度(使用[0,1]范围下的fragPosLight当坐标)
    
    // 取得当前片段在光源视角下的深度
    float currentDepth = projCoords.z;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);

    float shadow = 0.0;



    float pcf_range = 3;

    for(int i=0;i<N_SAMPLE;i++){
        float pcfDepth = texture(shadowMap, projCoords.xy + poissonDisk[i]*pcf_range * texelSize).r; 
        shadow += (currentDepth - bias > pcfDepth ? 1.0 : 0.0);     
    }
    shadow /=(N_SAMPLE);

    // for(int x = -pcf_range; x <= pcf_range; ++x)
    // {
    //     for(int y = -pcf_range; y <= pcf_range; ++y)
    //     {
    //         float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
           
    //         shadow += (currentDepth - bias > pcfDepth ? 1.0 : 0.0);        
    //     }    
    // }
    
    // 检查当前片段是否在阴影中
     
   // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    if(projCoords.z > 1.0 ||projCoords.z <= 0.0)
        shadow = 0.0;
    return shadow;
}



bool pbr= true;


void main()
{
    
    float deltaPos = length(LastFramePos.xyz);

    

    
    // diffuse 
    
    vec3 N = GetNormal();
    vec3 V =  normalize(viewPos - FragPos);
    vec3 R = reflect(-V, N);
    vec3 D = normalize(d_light.direction);
    float bias = 0.005;
    float shadow = d_light.shadowCasting== true? ShadowCalculation(FragPosLightSpace,bias): 0;  

    vec3 color;
    if(pbr){
        vec3 Lo = vec3(0.0);
        vec3 albedo = GetAlbedo();
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
        
        vec3 ambient = vec3(0.04) * albedo + vec3(0,0,0.01);
        color = ambient + (1-shadow)*Lo;


        vec3 Lp = vec3(0.0);
        for(int i=0;i<p_light_amount;i++){
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
                Lp += (kD * albedo / PI + specular) * radiance * NdotL;     

            }
        }
        color = color + (1-shadow*0.3f)*Lp;

        color = color / (color + vec3(1.0));
        color = pow(color, vec3(1.0/2.2));  
    }
    else{
        color = CalcDirLight(d_light,N,V,shadow);

            for(int i=0;i<p_light_amount;i++){
                if(p_light[i].enable==1){
                    color+=CalcPointLight(p_light[i],N,FragPos,V,0);
                }
            }
    }

    
    
    
    FragColor = vec4(color, 1.0);
    color1 = ObjID;

    ViewSpacePos = vec4(VSPos,LinearizeDepth(gl_FragCoord.z));
    ViewSpaceNormal = vec4(normalize(VSNorm) * 0.5 + 0.5, 1.0);
    OutNormal  = vec4(N,1);


    OutMaterial = vec4(material.metallic, material.roughness,deltaPos, (ObjID>=0?1.0f:0f));
}