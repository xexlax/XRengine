#type vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aTangent;
layout(location = 3) in vec2 aTexCoords;



uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;


void main()
{
    vec3 FragPos = vec3(u_Transform * vec4(aPos, 1.0));
    gl_Position = u_ViewProjection * vec4(FragPos, 1.0);

}


#type fragment
#version 330 core
layout(location = 0) out vec4 color;
layout(location = 1) out int color2;
out vec4 FragColor;

uniform int id;

void main()
{
    color2 = id;
}