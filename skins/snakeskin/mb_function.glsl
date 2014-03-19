#version 120

uniform vec4 balls[4];
uniform sampler2D storedValue;

varying vec2 world_coord, screen_coord;

float sqr(float x) { return x*x; }

void main(void) {
	float val = texture2D(storedValue, screen_coord).r;
	for (int i=0; i<balls.length(); ++i) {
		vec4 v = balls[i];
		vec2 c = v.xy - world_coord;
		val = val + v.w * sqr(sqr(v.z)) / sqr(sqr(c.x) + sqr(c.y));
	}
	gl_FragColor = vec4(val, 0, 0, 0);
}
