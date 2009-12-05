uniform sampler2D diffuse_map;
uniform sampler2D normal_map;

varying vec2 snake_coord, world_coord;
varying vec3 across_i, along_i;

const float density = 0.02;
const float min_a = 0.1, max_a = 0.25;

const float M_PI = 3.14159265358979;

void main(void) {
	vec3 across = normalize(across_i);
	vec3 along = normalize(along_i);

	float ang = acos(snake_coord[0]);
	float len_around = clamp(ang / M_PI, 0.0, 1.0);
	vec2 texture_coord = vec2(len_around * (max_a - min_a) + min_a, -snake_coord[1] * density);

	vec4 diffuse = texture2D(diffuse_map, texture_coord);
	vec4 bump_normal = (texture2D(normal_map, texture_coord) * 2.0 - 1.0) * vec4(-1, -1, 1, 0);

	// The length of the interpolated bump_normal can be used
	// as an estimate for the local variance in normals.
	float local_variance = 1.0 / length(bump_normal);

	bump_normal = normalize(bump_normal);

	float h = sqrt(1 - snake_coord[0]*snake_coord[0]);
	vec2 shape_normal = vec2(snake_coord[0], h);

	vec3 out_v = shape_normal[0] * across + shape_normal[1] * vec3(0, 0, 1);

	//vec4 normal = bump_normal;
	vec3 normal = bump_normal[0] * across + bump_normal[1] * along + bump_normal[2] * out_v;
	//normal = vec4(shape_normal[0], shape_normal[1], 0, 0);

	vec3 w = vec3(world_coord[0], world_coord[1], h*2.5);

	vec3 light = normalize(vec3(0, 0, 10) - w);

	vec4 final_color = diffuse * vec4(0.2, 0.2, 0.2, 1);

	float lambertTerm = dot(normal, light);

	if (lambertTerm > 0.0) {
		final_color += diffuse * lambertTerm;

		vec3 eye = vec3(0, 0, 1);
		vec3 r = reflect(-light, normal);

		// Using the local variance estimate to soften the specular
		// lighting in order to reduce specular aliasing
		float specular = pow(max(dot(r, eye), 0.0), 10.0) / (local_variance*local_variance*local_variance);
		final_color += 0.3 * specular;
	}

	gl_FragColor = final_color;
}
