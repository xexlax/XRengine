#type vertex
#version 450
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); 
}  
#type fragment
#version 450
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D ThisFrame;
uniform sampler2D LastFrame;
uniform sampler2D Speed;

float alpha = 0.5f;
void main()
{
    vec3 col1 = texture(LastFrame, TexCoords).rgb;
    vec3 col2 = texture(ThisFrame, TexCoords).rgb;

    float sp = texture(Speed,TexCoords).b;
    vec3 color = col2;
    if(sp<0.1f){
       color =  col1* alpha + (1.0f-alpha)*col2;
    }

    FragColor = vec4(color, 1.0);
} 