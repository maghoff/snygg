uniform sampler1D balls;

varying vec2 world_coord;

void main(void) {
	vec4 color = vec4(1, 0, 0, 1);
	vec4 no_color = vec4(0, 0, 0, 1);

	//f(x,y,z) = 1 / ((x − x0)2 + (y − y0)2 + (z − z0)2)

	float val = 0;
	for (int i=0; i<1; ++i) {
		vec2 c = vec2(0, 0) - world_coord;
		val = val + c.x*c.x + c.y*c.y;
	}

	// Use smoothstep?

	gl_FragColor = val < 5. ? color : no_color;
}
