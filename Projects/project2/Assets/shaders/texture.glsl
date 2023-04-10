#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
out vec2 v_TexCoord;
void main()
{
	v_TexCoord = a_TexCoord;
	gl_Position = vec4(aPos, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4  FragColor;
in vec2 v_TexCoord;

uniform sampler2D u_Texture;
void main()
{
	 FragColor = vec4(1,0,0,1);
	//color = texture(u_Texture, v_TexCoord);
}