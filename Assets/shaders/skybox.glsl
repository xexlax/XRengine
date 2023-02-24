#type vertex
#version 330 core
layout(location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 u_ViewProjection;


void main()
{
    TexCoords = aPos;
    vec4 pos =  u_ViewProjection * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}

#type fragment
#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{
    FragColor = texture(skybox, TexCoords);
}