#type vertex
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;


void main()
{
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); 
}  
#type fragment
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;




uniform sampler2D inColoring;
uniform sampler2D inSSAO;
uniform sampler2D inSSR;

uniform int SSAO_ON;
uniform int SSR_ON;




void main()
{
    

    vec3 color = texture(inColoring,TexCoords).rgb;

    if(SSAO_ON){
        color *= texture(inSSAO,TexCoords);
    }

    
    
    
    FragColor = vec4(color, 1.0);
}