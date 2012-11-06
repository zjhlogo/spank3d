#version 150

uniform float MAX_ITERATIONS = 50.0;
uniform float ZOOM = 1.0;
uniform vec2 CENTER_POS = vec2(0.0, 0.0);

uniform vec3 INNER_COLOR = vec3(0.0, 0.0, 0.0);
uniform vec3 OUTER_COLOR1 = vec3(0.0, 0.0, 0.1);
uniform vec3 OUTER_COLOR2 = vec3(0.0, 1.0, 0.0);

in vec3 v2f_position;

out vec4 f2d_color;

void main()
{
	float real = v2f_position.x * ZOOM + CENTER_POS.x;
	float imag = v2f_position.y * ZOOM + CENTER_POS.y;

	float Creal = real;
	float Cimag = imag;

	float r2 = 0.0;
	float iter;
	for (iter = 0.0; iter < MAX_ITERATIONS && r2 < 4.0; ++iter)
	{
		float tempreal = real;
		real = (tempreal * tempreal) - (imag * imag) + Creal;
		imag = 2.0 * tempreal * imag + Cimag;
		r2 = (real * real) + (imag * imag);
	}

	vec3 color;
	if (r2 < 4.0)
	{
		color = INNER_COLOR;
	}
	else
	{
		color = mix(OUTER_COLOR1, OUTER_COLOR2, fract(iter * 0.05));
	}

	f2d_color = vec4(color, 1.0);
}
