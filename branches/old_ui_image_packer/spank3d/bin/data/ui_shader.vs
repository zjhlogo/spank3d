#version 150

uniform mat4 u_matModelViewProj;

in vec4 p2v_pos_index;
in vec2 p2v_texCoord;
in vec4 p2v_color;
in vec4 p2v_clip_rect;

out vec2 v2f_pos;
out vec2 v2f_texCoord;
out vec4 v2f_color;
flat out ivec4 v2f_clipRect;
flat out int v2f_texIndex;

void main()
{
	ivec2 pos = ivec2(p2v_pos_index.xy);
	gl_Position = u_matModelViewProj * vec4(pos.x, pos.y, p2v_pos_index.z, 1.0);

	v2f_pos = pos;
	v2f_texCoord = p2v_texCoord;
	v2f_color = p2v_color;
	v2f_clipRect = ivec4(p2v_clip_rect);
	v2f_texIndex = int(p2v_pos_index.w);
}
