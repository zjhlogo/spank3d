#version 150

uniform vec3 u_baseColor = vec3(0.4, 0.4, 1.0);
uniform float u_mixRatio = 0.8;
uniform samplerCube u_texEnvMap;

in vec3 v2f_reflectDir;
in float v2f_lightIntensity;

out vec4 f2d_color;

void main()
{
	vec3 envColor = vec3(texture(u_texEnvMap, v2f_reflectDir));
	vec3 base = v2f_lightIntensity * u_baseColor;
	envColor = mix(envColor, base, u_mixRatio);
	f2d_color = vec4(envColor, 1.0);
}
