#version 150

uniform mat3 u_matModelView;
uniform mat4 u_matModelViewProj;
uniform mat3 u_matNormal;
uniform vec3 u_vLightPosition;

in vec3 p2v_position;
in vec2 p2v_texCoord;
in vec3 p2v_normal;

out vec3 v2f_reflectDir;
out float v2f_lightIntensity;

void main()
{
	vec3 vertNormal = u_matNormal * p2v_normal;
	vec3 vertexPos = vec3(u_matModelView * p2v_position);
	v2f_reflectDir = reflect(vertexPos, vertNormal);

	v2f_lightIntensity = max(dot(normalize(u_vLightPosition - vertexPos), vertNormal), 0.0);

	gl_Position = u_matModelViewProj * vec4(p2v_position, 1.0);
}
