#ifdef GL_ES
precision mediump float;
#endif

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

uniform vec4 color, ambient;

void main(void) {
	gl_FragColor = color_space_mapping(color * ambient);
}
