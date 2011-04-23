uniform int number_of_balls;
uniform sampler1D balls;
uniform sampler2D storedValue;

varying vec2 world_coord, screen_coord;

float sqr(float x) { return x*x; }

void main(void) {
	float val = texture2D(storedValue, screen_coord).r;
	for (int i=0; i<number_of_balls; ++i) {
		vec4 v = texture1D(balls, (float(i) + 0.5)/float(number_of_balls));
		vec2 c = v.xy - world_coord;
		val = val + v.w * sqr(sqr(v.z)) / sqr(sqr(c.x) + sqr(c.y));
	}
	gl_FragColor = vec4(val, 0, 0, 0);
}
