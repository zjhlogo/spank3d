uniform mat4 u_matModelViewProj;

attribute vec3 a_position;

void main()
{
	gl_Position = u_matModelViewProj * vec4(a_position, 1.0);
}
