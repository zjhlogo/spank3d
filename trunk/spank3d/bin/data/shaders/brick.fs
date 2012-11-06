#version 150

uniform vec3 u_vBrickColor = vec3(1.0, 0.3, 0.2);
uniform vec3 u_vMortarColor = vec3(0.85, 0.86, 0.84);
uniform vec2 u_vBrickSize = vec2(0.3, 0.15);
uniform vec2 u_vBrickPitch = vec2(0.9, 0.85);

in vec2 v2f_mcPosition;
in float v2f_lightIntensity;

out vec4 f2d_color;

void main()
{
	vec2 position = v2f_mcPosition / u_vBrickSize;

	if (fract(position.y * 0.5) > 0.5) position.x += 0.5;

	position = fract(position);
	vec2 useBrick = step(position, u_vBrickPitch);

	vec3 color = mix(u_vMortarColor, u_vBrickColor, useBrick.x * useBrick.y);
	color *= v2f_lightIntensity;

	f2d_color = vec4(color, 1.0);
}
