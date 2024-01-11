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
uniform sampler2D inID;

uniform bool SSAO_ON;
uniform bool SSAO_BLUR;
uniform bool SSR_ON;




void main()
{
    
    int id = texture(inID,TexCoords).r;
    vec3 color = texture(inColoring,TexCoords).rgb;
    if(id>0){
        if(SSAO_ON){

                //对SSAO进行模糊
                const int blurRange = 5;
                int n = 0;
                vec2 texelSize = 1.0 / vec2(textureSize(inSSAO, 0));
                float result = 0.0;
                for (int x = -blurRange; x < blurRange; x++) 
                {
                    for (int y = -blurRange; y < blurRange; y++) 
                    {
                        vec2 offset = vec2(float(x), float(y)) * texelSize;
                        result += texture(inSSAO, TexCoords + offset).r;
                        n++;
                    }
                }

                color *= result/float(n);


            }
    }
    

    
    
    
    FragColor = vec4(color, 1.0);
}