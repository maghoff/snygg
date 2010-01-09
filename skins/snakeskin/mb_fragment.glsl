uniform int number_of_balls;
uniform sampler1D balls;

varying vec2 world_coord;

float sqr(float x) { return x*x; }

void main(void) {
	float val = 0;
	for (int i=0; i<number_of_balls; ++i) {
		vec3 v = texture(balls, (float(i) + 0.5)/float(number_of_balls));
		vec2 c = vec2(v.x, v.y) - world_coord;
		val = val + sqr(sqr(v.z))/sqr(sqr(c.x) + sqr(c.y));
	}

	const vec4 color = vec4(1, 0, 0, 1);
	const vec4 back_color = vec4(0, 0, 0, 0);

	float color_f = smoothstep(1., 1.1, val);
	float back_color_f = 1 - color_f;

	gl_FragColor = color_f * color + back_color_f * back_color;
}
