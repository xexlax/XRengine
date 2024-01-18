
#type vertex
#version 450

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 inUV;

void main()
{
    inUV = aTexCoords;
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); 
}  


#type fragment

#version 450

uniform sampler2D samplerPositionDepth;
uniform sampler2D samplerNormal;
uniform sampler2D ssaoNoise;

const int SSAO_KERNEL_SIZE = 64;
const float SSAO_RADIUS = 1.5;

uniform vec4 UBOSSAOKernel[SSAO_KERNEL_SIZE];


uniform mat4 u_Projection;

in vec2 inUV;

layout(location = 0) out vec4 outFragColor;

void main() 
{
	//读取MRT
	vec3 fragPos = texture(samplerPositionDepth, inUV).rgb;
	vec3 normal = normalize(texture(samplerNormal, inUV).rgb * 2.0 - 1.0);

	//查找噪声图，获取随机方向
	ivec2 texDim = textureSize(samplerPositionDepth, 0); 
	ivec2 noiseDim = textureSize(ssaoNoise, 0);
	const vec2 noiseUV = vec2(float(texDim.x)/float(noiseDim.x), float(texDim.y)/(noiseDim.y)) * inUV;  
	vec3 randomVec = texture(ssaoNoise, noiseUV).xyz * 2.0 - 1.0;
	
	//施密特正交化
	vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
	vec3 bitangent = cross(tangent, normal);
	mat3 TBN = mat3(tangent, bitangent, normal);


	float occlusion = 0.0f;
	const float bias = 0.1f;
	
	for(int i = 0; i < SSAO_KERNEL_SIZE; i++)
	{	
		//计算kernel+noise得到采样向量
		vec3 samplePos = TBN * UBOSSAOKernel[i].xyz; 
		samplePos = fragPos + samplePos * SSAO_RADIUS; 
		
		//透视除法获取屏幕空间点
		vec4 offset = vec4(samplePos, 1.0f);
		offset = u_Projection * offset; 
		offset.xyz /= offset.w; 
		offset.xyz = offset.xyz * 0.5f + 0.5f; 
		
		float sampleDepth = -texture(samplerPositionDepth, offset.xy).w; 

		//平滑插值
		float rangeCheck = smoothstep(0.0f, 1.0f, SSAO_RADIUS / abs(fragPos.z - sampleDepth));
		occlusion += 5*(sampleDepth >= samplePos.z + bias ? 1.0f : 0.0f) * rangeCheck;  
		
		       
	}
	
	occlusion = 1.0 - (occlusion / float(SSAO_KERNEL_SIZE));
	outFragColor =vec4(occlusion,occlusion,occlusion,1); 
	
}

