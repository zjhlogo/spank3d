#version 150

uniform mat4 u_matModelViewProj;

in vec2 p2v_position;
in vec2 p2v_texCoord;

out vec2 v2g_texCoord;

void main()
{
	gl_Position = u_matModelViewProj * vec4(p2v_position, -10.0, 1.0);
	v2g_texCoord = p2v_texCoord;
}
