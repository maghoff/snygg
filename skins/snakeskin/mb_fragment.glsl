uniform sampler1D balls;

varying vec2 world_coord;

void main(void) {
	//f(x,y,z) = 1 / ((x − x0)2 + (y − y0)2 + (z − z0)2)

	float val = 0;
	for (int i=0; i<1; ++i) {
		vec2 c = vec2(0, 0) - world_coord;
		val = val + 300/(c.x*c.x + c.y*c.y);

		c = vec2(10, 0) - world_coord;
		val = val + 300/(c.x*c.x + c.y*c.y);
	}

	const vec4 color = vec4(1, 0, 0, 1);
	const vec4 back_color = vec4(0, 0, 0, 1);

	float color_f = smoothstep(10., 11., val);
	float back_color_f = 1 - color_f;

	gl_FragColor = color_f * color + back_color_f * back_color;
}
