#version 150

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

in vec2 v2g_texCoord[3];

out vec2 g2f_texCoord;

void main()
{
	gl_Position = gl_in[0].gl_Position;
	g2f_texCoord = v2g_texCoord[0];
	EmitVertex();

	gl_Position = gl_in[1].gl_Position;
	g2f_texCoord = v2g_texCoord[1];
	EmitVertex();

	gl_Position = gl_in[2].gl_Position;
	g2f_texCoord = v2g_texCoord[2];
	EmitVertex();

	EndPrimitive();
}
