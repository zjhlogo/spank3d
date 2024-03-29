#version 150

uniform mat3 u_matView;
uniform mat3 u_matModelView;
uniform mat4 u_matModelViewProj;
uniform mat3 u_matNormal;
uniform mat4 u_matDepthBiasModelViewProj;

uniform vec3 u_vLightPosition;

in vec3 p2v_position;
in vec2 p2v_texCoord;
in vec3 p2v_normal;

out float v2f_diffuse;
out float v2f_specular;
out vec4 v2f_shadowCoord;

void main()
{
	vec3 vertPos = u_matModelView * p2v_position;
	vec3 lightPos = u_matView * u_vLightPosition;

	vec3 lightDir = normalize(vertPos - lightPos);
	vec3 vertNormal = normalize(u_matNormal * p2v_normal);

	vec3 eyeDir = normalize(-vertPos);
	vec3 reflectDir = reflect(lightDir, vertNormal);

	v2f_diffuse = clamp(dot(-lightDir, vertNormal), 0.0, 1.0);
	v2f_specular = clamp(dot(reflectDir, eyeDir), 0.0, 1.0);

	v2f_shadowCoord = u_matDepthBiasModelViewProj * vec4(p2v_position, 1.0);
	gl_Position = u_matModelViewProj * vec4(p2v_position, 1.0);
}
