#version 330 core
out vec4 FragColor;

in vec4 vColor;
in vec3 v_Position;
 
void main()
{
    FragColor = vec4(v_Position * 0.5 + 0.5, 1.0);
    FragColor = vColor;
}