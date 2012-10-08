#version 150

uniform mat4 u_matModelViewProj;

in vec2 p2v_position;
in vec2 p2v_texCoord;

out vec2 v2f_texCoord;

void main()
{
	gl_Position = u_matModelViewProj * vec4(int(p2v_position.x), int(p2v_position.y), 0.0, 1.0);
	v2f_texCoord = p2v_texCoord;
}
