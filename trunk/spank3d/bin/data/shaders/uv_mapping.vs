#version 150

in vec3 p2v_position;
in vec2 p2v_texCoord;
in vec3 p2v_normal;

void main()
{
	vec2 texCoord = p2v_texCoord * 2.0 - 1.0;
	gl_Position = vec4(texCoord.xy, 1.0, 1.0);
}
