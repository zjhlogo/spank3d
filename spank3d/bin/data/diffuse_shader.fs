#version 150

const vec3 AMBIENT_COLOR = vec3(0.1, 0.1, 0.1);
const vec3 DIFFUSE_COLOR = vec3(1.0, 1.0, 1.0);
const vec3 SPECULAR_COLOR = vec3(1.0, 1.0, 1.0);

in float v2f_diffuse;
in float v2f_specular;

out vec4 f2d_color;

void main()
{
	f2d_color = vec4(AMBIENT_COLOR+DIFFUSE_COLOR*v2f_diffuse+SPECULAR_COLOR*pow(v2f_specular, 8), 1.0);
}
