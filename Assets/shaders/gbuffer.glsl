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

uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;


void main()
{
    FragPos = vec3(u_View* u_Transform * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(u_View* u_Transform))) * aNormal;
    TexCoords = aTexCoords;
    Tangent = aTangent;
    gl_Position = u_ViewProjection * vec4(FragPos, 1.0);
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
}


#type fragment
#version 450
layout(location = 0) out vec4 outPosition;
layout(location = 1) out vec4 outNormal;
layout(location = 2) out vec4 outAlbedo;
layout(location = 3) out vec4 outSpecular;
layout(location = 4) out int outID;
//id
//layout(location = 4) out int color1;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
in vec3 Tangent;
in vec4 FragPosLightSpace;

uniform vec3 viewPos;


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


uniform Material material;

uniform int ObjID;


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



void main()
{
    
    
    // diffuse 
    outAlbedo = vec4(GetAlbedo(),1.0f);
    outPosition = vec4(FragPos,1.0f);
    outNormal = vec4(GetNormal(),1.0f);
    outSpecular = vec4(GetSpecular(),1.0f);
    outID = ObjID;
    //color1 = ObjID;
}