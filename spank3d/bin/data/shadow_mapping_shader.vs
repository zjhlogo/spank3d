#version 150

uniform mat3 u_matModelView;
uniform mat4 u_matModelViewProj;
uniform mat3 u_matNormal;
uniform mat4 u_matDepthBiasModelViewProj;

in vec3 p2v_position;
in vec2 p2v_texCoord;
in vec3 p2v_normal;

out vec4 v2f_shadowCoord;
out float v2f_lightIntensity;

void main()
{
	vec3 vertexPos = vec3(u_matModelView * p2v_position);
	vec3 vertNormal = u_matNormal * p2v_normal;
	vec3 viewVec = normalize(-vertexPos);

	v2f_shadowCoord = u_matDepthBiasModelViewProj * vec4(p2v_position, 1.0);
	v2f_lightIntensity = max(dot(viewVec, vertNormal), 0.0);
	gl_Position = u_matModelViewProj * vec4(p2v_position, 1.0);
}
