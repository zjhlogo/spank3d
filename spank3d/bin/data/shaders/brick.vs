#version 150

const float SPECULAR = 0.3;
const float DIFFUSE = 1.0 - SPECULAR;

uniform mat3 u_matModelView;
uniform mat4 u_matModelViewProj;
uniform mat3 u_matNormal;
uniform vec3 u_vLightPosition;

in vec3 p2v_position;
in vec2 p2v_texCoord;
in vec3 p2v_normal;

out vec2 v2f_mcPosition;
out float v2f_lightIntensity;

void main()
{
	vec3 vertexPos = vec3(u_matModelView * p2v_position);
	v2f_mcPosition = vertexPos.xy;

	vec3 lightVec = normalize(u_vLightPosition - vertexPos);
	vec3 vertNormal = u_matNormal * p2v_normal;

	vec3 reflectVec = reflect(-lightVec, vertNormal);
	vec3 viewVec = normalize(vertexPos);

	float diffuse = max(dot(lightVec, vertNormal), 0.0);
	float specular = 0.0;
	if (diffuse > 0.0)
	{
		specular = max(dot(reflectVec, viewVec), 0.0);
		specular = pow(specular, 16.0);
	}

	v2f_lightIntensity = DIFFUSE * diffuse + SPECULAR * specular;

	gl_Position = u_matModelViewProj * vec4(p2v_position, 1.0);
}
