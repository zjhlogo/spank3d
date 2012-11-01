#version 150

uniform sampler2D u_texture;

in vec2 v2f_texCoord;
in vec3 v2f_normal;

out vec4 f2d_color;

void main()
{
	vec4 diffuse = texture2D(u_texture, v2f_texCoord);
	f2d_color = diffuse + vec4(0.2, 0.2, 0.2, 1.0);
}
