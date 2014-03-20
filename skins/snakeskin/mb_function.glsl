#ifdef GL_ES
precision mediump float;
#endif

const int number_of_balls = 4;

uniform vec4 balls[number_of_balls];
uniform sampler2D storedValue;

uniform float delta_x;
varying vec2 world_coord, screen_coord;

float sqr(float x) { return x*x; }

void main(void) {
	vec4 val = texture2D(storedValue, screen_coord);
	for (int j=0; j<4; ++j) {
		for (int i=0; i<number_of_balls; ++i) {
			vec4 v = balls[i];
			vec2 c = v.xy - (world_coord + vec2(float(j) * delta_x, 0));
			val[j] = val[j] + v.w * sqr(sqr(v.z)) / sqr(sqr(c.x) + sqr(c.y));
		}
	}
	gl_FragColor = val;
}
