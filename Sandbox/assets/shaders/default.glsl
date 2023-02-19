
#type vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec4 vColor;
out vec3 v_Position;

void main()
{
    v_Position = aPos;
    vColor = aColor;
    gl_Position = u_ViewProjection * u_Transform  * vec4(aPos, 1.0);	
}

#type fragment
#version 330 core
out vec4 FragColor;

in vec4 vColor;
in vec3 v_Position;

void main()
{
    FragColor = vec4(v_Position * 0.5 + 0.5, 1.0);
    FragColor = vColor;
}