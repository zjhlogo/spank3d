#version 150

uniform mat4 u_matModelViewProj;

in vec4 p2v_pos_uv;
in vec4 p2v_clip_rect;

out vec4 v2f_posUv;
out vec4 v2f_clipRect;

void main()
{
	gl_Position = u_matModelViewProj * vec4(int(p2v_pos_uv.x), int(p2v_pos_uv.y), 0.0, 1.0);

	v2f_posUv = p2v_pos_uv;
	v2f_clipRect = p2v_clip_rect;
}
