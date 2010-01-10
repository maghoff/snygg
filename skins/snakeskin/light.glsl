const vec4 ambient = vec4(0.2, 0.2, 0.2, 1);

vec4 directional_light(
	vec3 normal,
	vec3 light,
	vec4 diffuse,
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
		float specular = pow(max(dot(r, eye), 0.0), 5.0) / pow(local_variance, 3.0);
		final_color += 0.3 * specular;
	}

	return final_color;
}
