#version 150

uniform sampler2D u_texture;

in vec4 v2f_posUv;
in vec4 v2f_clipRect;

out vec4 f2d_color;

void main()
{
	if (v2f_posUv.x < v2f_clipRect.x
	|| v2f_posUv.y < v2f_clipRect.y
	|| v2f_posUv.x > v2f_clipRect.z
	|| v2f_posUv.y > v2f_clipRect.w)
	{
		discard;
	}

	f2d_color = texture2D(u_texture, v2f_posUv.zw);
}
