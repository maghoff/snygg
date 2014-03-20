#ifdef GL_ES
precision mediump float;
#endif

#ifndef GL_ES
uniform vec4 ambient;
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

vec4 directional_light(
	vec3 normal,
	vec3 light,
	vec4 diffuse,
	float phong_exponent,
	float local_variance
) {
	vec4 final_color = vec4(0, 0, 0, 1);

	float lambertTerm = dot(normal, light);

	if (lambertTerm > 0.0) {
		final_color += 0.8 * diffuse * lambertTerm;

		vec3 eye = vec3(0, 0, 1);
		vec3 r = reflect(-light, normal);

		// Using the local variance estimate to soften the specular
		// lighting in order to reduce specular aliasing
		float specular = pow(max(dot(r, eye), 0.0), phong_exponent) / pow(local_variance, 3.0);
		final_color += 0.3 * specular;
	}

	return final_color;
}
