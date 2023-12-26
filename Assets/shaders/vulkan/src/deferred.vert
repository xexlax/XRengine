#version 450


layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aTangent;
layout(location = 3) in vec2 aTexCoords;

layout(location = 0) out vec3 FragPos;
layout(location = 1) out vec3 Normal;
layout(location = 2) out vec3 Tangent;
layout(location = 3) out vec2 TexCoords;


layout(binding = 0) uniform GlobalUBO {
    mat4 view;
    mat4 proj;
    vec3 camPos;
} ubo;

layout(push_constant) uniform Push {
    mat4 model;
    int id;
} push;

void main() 
{
  	FragPos = vec3(push.model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(push.model))) * aNormal;
    TexCoords = aTexCoords;
    Tangent = aTangent;
    gl_Position = ubo.proj * ubo.view * push.model * vec4(FragPos, 1.0);

}
