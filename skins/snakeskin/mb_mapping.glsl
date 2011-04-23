uniform sampler2D storedValue;
varying vec2 screen_coord;

void main(void) {
	float val = texture2D(storedValue, screen_coord).r;

	const vec4 color = vec4(0.7, 0, 0, 1);
	const vec4 back_color = vec4(0.7, 0, 0, 0);

	float color_f = smoothstep(1., 1.1, val);
	float back_color_f = 1. - color_f;

	gl_FragColor = color_f * color + back_color_f * back_color;
}
