#version 150

in vec3 g2f_dist;

out vec4 f2h_color;

void main()
{
	float nearD = min(min(g2f_dist.x, g2f_dist.y), g2f_dist.z);
	float edgeIntensity = exp2(-1.0 * nearD * nearD);

	f2h_color = edgeIntensity * vec4(0.1, 0.1, 0.1, 1.0) + (1.0 - edgeIntensity) * vec4(0.4, 0.4, 0.8, 1.0);
}
