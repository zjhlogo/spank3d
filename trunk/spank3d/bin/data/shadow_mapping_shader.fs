#version 150

const vec3 DIFFUSE_COLOR = vec3(1.0, 1.0, 1.0);

uniform sampler2D u_shadowMap;

in vec4 v2f_shadowCoord;
in float v2f_lightIntensity;

out vec4 f2d_color;

void main()
{
	float visibility = texture(u_shadowMap, v2f_shadowCoord.xy).r;
	f2d_color = vec4(DIFFUSE_COLOR*v2f_lightIntensity*visibility, 1.0);
}
