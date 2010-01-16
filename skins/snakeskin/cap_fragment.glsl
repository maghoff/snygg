uniform sampler2D diffuse_map;
uniform sampler2D normal_map;

varying vec2 circle_coord, world_coord;
varying vec3 across_i, along_i;

const float M_PI = 3.14159265358979;

const float density = 0.04;
const float min_a = 0.1, max_a = 0.45;

extern const vec4 ambient;
vec4 directional_light(vec3 normal, vec3 light, vec4 diffuse, float local_variance);

mat3 calculate_world_from_skin(in mat3 world_from_snake, in vec2 circle_coord) {
	float ang1 = asin(circle_coord.y);
	float width = abs(cos(ang1));
	float ang = acos(clamp(circle_coord.x/width, -1.0, 1.0));

	float h = sqrt(1 - circle_coord.x*circle_coord.x - circle_coord.y*circle_coord.y);

	vec3 shape_normal = vec3(circle_coord.x, circle_coord.y, h);
	vec3 zdir = world_from_snake * shape_normal;

	vec3 xdir = vec3(sin(ang), 0, -cos(ang));

	float ang2 = asin(circle_coord.x);
	float yang = acos(circle_coord.y / abs(cos(ang2)));
	vec3 ydir = vec3(0, -sin(yang), cos(yang));

	xdir = world_from_snake * xdir;
	ydir = world_from_snake * ydir;

	return mat3(xdir, ydir, zdir);
}

void main(void) {
	mat3 world_from_snake = mat3(normalize(across_i), normalize(along_i), vec3(0, 0, 1));

	// 1: Calculate texture_coord

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

	// 2: Look up diffuse color
	vec4 diffuse = texture2D(diffuse_map, texture_coord);

	// 3: Calculate transformation matrix from skin coordinates to world coordinates
	mat3 world_from_skin = calculate_world_from_skin(world_from_snake, circle_coord);

	// 4: Look up and transform the normal from the bump map
	vec3 bump_normal = (vec3(texture2D(normal_map, texture_coord)) * 2.0 - 1.0) * vec3(-1, -1, 1);

	// The length of the interpolated bump_normal can be used as an estimate for
	// the local variance in normals, and can be used to reduced specular aliasing.
	float local_variance = 1.0 / length(bump_normal);

	bump_normal = normalize(bump_normal);

	vec3 normal = world_from_skin * vec3(bump_normal);
	//vec3 normal = world_from_snake * vec3(0, 0, 1);

	// 5: Set up the light
	float h = sqrt(1 - circle_coord.x*circle_coord.x - circle_coord.y*circle_coord.y);
	vec3 w = vec3(world_coord[0], world_coord[1], h*2.5);
	vec3 light = normalize(vec3(0, 0, 3) - w);

	// 6: Calculate the lighting
	gl_FragColor =
		diffuse * ambient +
		directional_light(normal, light, diffuse, local_variance)
	;
}
