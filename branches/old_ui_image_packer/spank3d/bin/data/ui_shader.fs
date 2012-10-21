#version 150

uniform sampler2D u_texture1;
uniform sampler2D u_texture2;
uniform sampler2D u_texture3;
uniform sampler2D u_texture4;

in vec2 v2f_pos;
in vec2 v2f_texCoord;
in vec4 v2f_color;
flat in ivec4 v2f_clipRect;
flat in int v2f_texIndex;

out vec4 f2d_color;

void main()
{
	if (v2f_pos.x < v2f_clipRect.x
	|| v2f_pos.y < v2f_clipRect.y
	|| v2f_pos.x > v2f_clipRect.z
	|| v2f_pos.y > v2f_clipRect.w)
	{
		discard;
	}

	switch (v2f_texIndex)
	{
	case 0:
		f2d_color = texture2D(u_texture1, v2f_texCoord) * v2f_color;
		break;
	case 1:
		f2d_color = texture2D(u_texture2, v2f_texCoord) * v2f_color;
		break;
	case 2:
		f2d_color = texture2D(u_texture3, v2f_texCoord) * v2f_color;
		break;
	case 3:
		f2d_color = texture2D(u_texture4, v2f_texCoord) * v2f_color;
		break;
	default:
		f2d_color = texture2D(u_texture1, v2f_texCoord) * v2f_color;
		break;
	}
}
