#version 150

uniform mat4 u_matModelViewProj;

in vec3 p2v_position;
in vec2 p2v_texCoord;
in vec3 p2v_normal;

void main()
{
	gl_Position = u_matModelViewProj * vec4(p2v_position, 1.0);
}
