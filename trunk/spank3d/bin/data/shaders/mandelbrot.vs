#version 150

uniform mat3 u_matModelView;
uniform mat4 u_matModelViewProj;

in vec3 p2v_position;

out vec3 v2f_position;

void main()
{
	v2f_position = p2v_position;
	gl_Position = u_matModelViewProj * vec4(p2v_position, 1.0);
}
