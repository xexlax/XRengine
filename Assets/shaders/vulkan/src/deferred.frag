#version 450

layout (location = 0) out vec4 outPosition;
layout (location = 1) out vec4 outNormal;
layout (location = 2) out vec4 outAlbedo;

layout(location = 0) in vec3 FragPos;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec3 Tangent;
layout(location = 3) in vec2 TexCoords;
//layout(location = 4) in vec4 FragPosLightSpace;

layout(binding = 1) uniform MaterialUBO{
    int enable_albedo;
    int enable_specularTex;
    int enable_bumpTex;
    float metallic;
    float roughness;
}material;

layout(binding = 2) uniform sampler2D albedoMap;
layout(binding = 3) uniform sampler2D specularMap;
layout(binding = 4) uniform sampler2D normalMap;

layout(push_constant) uniform Push {
    mat4 model;
    int id;
} push;



void main() 
{
	outPosition = vec4(FragPos, 1.0);

	

	// Calculate normal in tangent space
	vec3 N = normalize(Normal);
	
	if(material.enable_bumpTex>0){
		vec3 T = normalize(Tangent);
		vec3 B = cross(N, T);
		mat3 TBN = mat3(T, B, N);
		vec3 tnorm = TBN * normalize(texture(normalMap, TexCoords).xyz * 2.0 - vec3(1.0));
		outNormal = vec4(tnorm, 1.0);
	}
	else outNormal = vec4(N,1.0);
	
	if(material.enable_albedo>0)
	outAlbedo = texture(albedoMap, TexCoords);
	else outAlbedo = vec4(1.0); 
}