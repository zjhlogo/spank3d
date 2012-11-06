#version 150

uniform sampler2D u_texture;

in vec3 g2f_dist;
in vec2 g2f_texCoord;
in vec3 g2f_normal;

out vec4 f2d_color;

void main()
{
	float nearD = min(min(g2f_dist.x, g2f_dist.y), g2f_dist.z);
	float edgeIntensity = exp2(-1.0 * nearD * nearD);

	vec4 diffuse = texture(u_texture, g2f_texCoord);
	f2d_color = edgeIntensity * vec4(0.1, 0.1, 0.1, 1.0) + (1.0 - edgeIntensity) * diffuse;
}
