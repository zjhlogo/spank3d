#version 150

const vec3 AMBIENT_COLOR = vec3(0.1, 0.1, 0.1);
const vec3 SPECULAR_COLOR = vec3(1.0, 1.0, 1.0);

uniform sampler2D u_texture;

in vec4 v2f_texCoord_diffuse_specular;

out vec4 f2d_color;

void main()
{
	vec4 diffuse_color = texture(u_texture, v2f_texCoord_diffuse_specular.xy);
	f2d_color = vec4(AMBIENT_COLOR+diffuse_color.xyz*v2f_texCoord_diffuse_specular.z+SPECULAR_COLOR*pow(v2f_texCoord_diffuse_specular.w, 8), 1.0);
}
