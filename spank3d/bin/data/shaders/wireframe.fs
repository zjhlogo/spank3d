#version 150

const vec3 DIFFUSE_COLOR = vec3(1.0, 1.0, 1.0);

in vec3 g2f_dist;
in vec2 g2f_texCoord;
in vec3 g2f_normal;

out vec4 f2d_color;

void main()
{
	float nearD = min(min(g2f_dist.x, g2f_dist.y), g2f_dist.z);
	float edgeIntensity = exp2(-1.0 * nearD * nearD);
	f2d_color = vec4(edgeIntensity * DIFFUSE_COLOR, 1.0);
}
