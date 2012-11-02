#version 150

const vec3 DIFFUSE_COLOR = vec3(1.0, 1.0, 1.0);

in float v2f_lightIntensity;

out vec4 f2d_color;

void main()
{
	f2d_color = vec4(DIFFUSE_COLOR*v2f_lightIntensity, 1.0);
}
