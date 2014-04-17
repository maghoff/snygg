#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D storedValue;
varying vec2 screen_coord, world_coord;
uniform float screen_width;

uniform vec4 ambient;

#ifdef GL_ES
vec4 color_space_mapping(vec4 linear) {
	vec4 res;
	res.a = linear.a;

	for (int i=0; i<3; ++i) {
		float cl = linear[i];
		if (cl < 0.0031308) res[i] = 12.92*cl;
		else res[i] = 1.055 * pow(cl, 0.41666) - 0.055;
	}

	return res;
}
#else
vec4 color_space_mapping(vec4 linear) { return linear; }
#endif

float sample(vec2 p) {
	float j = mod(floor(p.x * screen_width), 4.);
	vec4 vals = texture2D(storedValue, (p - vec2(j / screen_width, 0)));
	return vals[int(j)];
}

void main(void) {
	float val = sample(screen_coord);
	float alpha = smoothstep(1., 1.1, val);

	const vec4 diffuse = vec4(0.7, 0, 0, 1);
	vec4 color = diffuse * ambient;
	gl_FragColor = color_space_mapping(vec4(color.rgb, alpha));
}
