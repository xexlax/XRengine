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
#version 450
out vec4 FragColor;


in vec2 TexCoords;




uniform sampler2D inColoring;
uniform sampler2D inSSAO;
uniform sampler2D inSSR;
uniform sampler2D inMat;

uniform bool SSAO_ON;
uniform bool SSR_ON;

uniform float Brightness;
uniform float Saturation;
uniform float Contrast;

uniform float MotionBlur;
uniform float DepthOfField;
uniform float Blooming_Strength;
uniform float Blooming_Threshold;

uniform float Vignette=0;


#define N_SAMPLE 16
const vec2 poissonDisk[16] = {
    vec2( -0.94201624, -0.39906216 ),
    vec2( 0.94558609, -0.76890725 ),
    vec2( -0.094184101, -0.92938870 ),
    vec2( 0.34495938, 0.29387760 ),
    vec2( -0.91588581, 0.45771432 ),
    vec2( -0.81544232, -0.87912464 ),
    vec2( -0.38277543, 0.27676845 ),
    vec2( 0.97484398, 0.75648379 ),
    vec2( 0.44323325, -0.97511554 ),
    vec2( 0.53742981, -0.47373420 ),
    vec2( -0.26496911, -0.41893023 ),
    vec2( 0.79197514, 0.19090188 ),
    vec2( -0.24188840, 0.99706507 ),
    vec2( -0.81409955, 0.91437590 ),
    vec2( 0.19984126, 0.78641367 ),
    vec2( 0.14383161, -0.14100790 )
};



void main()
{
    
    

    float sky = texture(inMat,TexCoords).a;
    float speed = texture(inMat,TexCoords).b;
    vec3 color = texture(inColoring,TexCoords).rgb;
    float metallic = texture(inMat,TexCoords).r;
    float roughness = texture(inMat,TexCoords).g;
    
    if(true){

        if(sky>0.5f){

            if(SSR_ON){
                
                vec4 ssr = vec4(0,0,0,0);
                if(roughness>0){

                    const float blurRange = 1;
                    
                    vec2 texelSize = 1.0 / vec2(textureSize(inSSAO, 0));
                    



                    
                    for (int i=0;i<N_SAMPLE;i++) 
                    {
                            vec2 offset = poissonDisk[i]*blurRange * texelSize;
                            
                            ssr += texture(inSSR, TexCoords + offset);
                            
                    }
                    ssr/= N_SAMPLE;

                }
                else ssr = texture(inSSR,TexCoords);


                if(metallic>0.1f)
                color =  metallic * ssr.rgb * ssr.a  + (1-metallic) *color ;

                
            }
            if(SSAO_ON){

                    //对SSAO进行模糊
                    const float blurRange = 5;
                    
                    vec2 texelSize = 1.0 / vec2(textureSize(inSSAO, 0));
                    float result = 0.0;



                    
                    for (int i=0;i<N_SAMPLE;i++) 
                    {
                            vec2 offset = poissonDisk[i]*blurRange * texelSize;
                            
                            result += texture(inSSAO, TexCoords + offset).r ;
                            
                    }
                    

                    color *= result/float(N_SAMPLE);


            }
        }


        
        float luminance = 0.2125* color.r +0.7154* color.g +0.0721* color.b;
        vec3 lumCol= vec3(luminance,luminance,luminance);
        vec3 avgCol = vec3(0.5,0.5,0.5);
        color*= Brightness;

        color = lumCol*(1-Saturation) + color* Saturation;

        color = avgCol*(1-Contrast) + color* Contrast;


        color *= min(1,(2-abs(TexCoords.x-0.5f)-abs(TexCoords.y-0.5f))*(1-Vignette)) ;
    }

        
    
    

    
    
    
    FragColor = vec4(color, 1.0);
}