#version 150

uniform sampler2D u_texture;

in vec2 g2f_texCoord;

out vec4 f2d_color;

void main()
{
	f2d_color = texture2D(u_texture, g2f_texCoord);
}
