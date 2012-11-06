#version 150

const vec3 AMBIENT_COLOR = vec3(0.1, 0.1, 0.1);
const vec3 DIFFUSE_COLOR = vec3(1.0, 1.0, 1.0);
const vec3 SPECULAR_COLOR = vec3(1.0, 1.0, 1.0);

uniform sampler2D u_shadowMap;

in float v2f_diffuse;
in float v2f_specular;
in vec4 v2f_shadowCoord;

out vec4 f2d_color;

void main()
{
	float visibility = 1.0;
	if (texture(u_shadowMap, v2f_shadowCoord.xy).z < v2f_shadowCoord.z - 0.005) visibility = 0.5;
	vec3 color = AMBIENT_COLOR+DIFFUSE_COLOR*v2f_diffuse+SPECULAR_COLOR*pow(v2f_specular, 8);
	f2d_color = vec4(color*visibility, 1.0);
}
