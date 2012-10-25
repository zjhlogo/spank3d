#version 150

const float SPECULAR = 0.3;
const float DIFFUSE = 1.0 - SPECULAR;

uniform mat4 u_matModelView;
uniform mat4 u_matModelViewProj;
uniform vec3 u_vLightPosition;

in vec3 p2v_position;
in vec2 p2v_texCoord;
in vec3 p2v_normal;

out vec2 v2f_mcPosition;
//out float v2f_lightIntensity;

void main()
{
	gl_Position = u_matModelViewProj * vec4(p2v_position, 1.0);
	v2f_mcPosition = gl_Position.xy;

//	vec3 ecPosition = (u_matModelView * vec4(p2v_position, 1.0)).xyz;
//	vec3 lightVec = normalize(u_vLightPosition - ecPosition);
//	vec3 normalVec = normalize((u_matModelView * vec4(p2v_normal, 1.0)).xyz);
//
//	vec3 reflectVec = reflect(-lightVec, normalVec);
//	vec3 viewVec = normalize(-ecPosition);
//
//	float diffuse = max(dot(lightVec, normalVec), 0.0);
//	float specular = 0.0;
//	if (diffuse > 0.0)
//	{
//		specular = max(dot(reflectVec, viewVec), 0.0);
//		specular = pow(specular, 16.0);
//	}
//
//	v2f_lightIntensity = DIFFUSE * diffuse + SPECULAR * specular;
}
