uniform sampler2D diffuse_map;
uniform sampler2D normal_map;

varying vec2 circle_coord, world_coord;
varying vec3 across_i, along_i;

const float M_PI = 3.14159265358979;

const float density = 0.04;
const float min_a = 0.1, max_a = 0.45;

const vec4 ambient = vec4(0.2, 0.2, 0.2, 1);

vec4 directional_light(
	vec3 normal,
	vec3 light,
	vec4 diffuse,
	float local_variance
) {
	vec4 final_color = vec4(0, 0, 0, 0);

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

void main(void) {
	vec3 across = normalize(across_i);
	vec3 along = normalize(along_i);

	float ang1 = asin(circle_coord.y);
	float width = abs(cos(ang1));
	float ang = acos(clamp(circle_coord.x/width, -1.0, 1.0));
	float len_around_x = ((ang / M_PI) - 0.5) * 2 * width;
	float len_around_y = ((acos(circle_coord.y) / M_PI) - 0.5) * 2 * 2.5 * M_PI;
	vec2 texture_coord =
		vec2(
			(len_around_x / 2) * (max_a - min_a) + ((max_a - min_a) / 2)
		,
			(len_around_y / 2) * density
		)
	;

	vec4 diffuse = texture2D(diffuse_map, texture_coord);
	vec4 bump_normal = (texture2D(normal_map, texture_coord) * 2.0 - 1.0) * vec4(-1, -1, 1, 0);

	// The length of the interpolated bump_normal can be used
	// as an estimate for the local variance in normals.
	float local_variance = 1.0 / length(bump_normal);

	bump_normal = normalize(bump_normal);

	float h = sqrt(1 - circle_coord.x*circle_coord.x - circle_coord.y*circle_coord.y);

	vec3 shape_normal = vec3(circle_coord[0], circle_coord[1], h);
	vec3 out_v = shape_normal[0] * across + shape_normal[1] * along + shape_normal[2] * vec3(0, 0, 1);

	vec3 xdir = along, ydir = across;

	xdir = vec3(sin(ang), 0, -cos(ang));

	float ang2 = asin(circle_coord.x);
	float yang = acos(circle_coord.y /abs(cos(ang2)));
	ydir = vec3(0, -sin(yang), cos(yang));

	vec3 normal = normalize(
		bump_normal.x * xdir +
		bump_normal.y * ydir +
		bump_normal.z * out_v
	);

	vec3 w = vec3(world_coord[0], world_coord[1], h*2.5);
	vec3 light = normalize(vec3(0, 0, 3) - w);

	gl_FragColor =
		diffuse * ambient +
		directional_light(normal, light, diffuse, local_variance)
	;
}
