#type vertex
#version 100
precision highp float;

attribute vec4 a_position;
attribute vec3 a_normal;
attribute vec2 a_uv;

uniform mat4 u_mvpMtx;

void main()
{
	gl_Position = u_mvpMtx * a_position;
	gl_PointSize = 4.0;
}
#type fragment
#version 100
precision highp float;

uniform vec4 u_color;

void main()
{
	//vec4 col = texture2D(u_texture, v_uv);
	//gl_FragColor = col * u_color;
	gl_FragColor = vec4(1.0);
}