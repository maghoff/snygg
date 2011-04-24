uniform sampler2D diffuse_map;
uniform sampler2D normal_map;
uniform vec4 ambient;

varying vec2 circle_coord, world_coord;
varying vec3 across_i, along_i;
varying float b;

const float M_PI = 3.14159265358979;

const float density = 0.04;
const float min_a = 0.1, max_a = 0.45;

vec4 directional_light(vec3 normal, vec3 light, vec4 diffuse, float local_variance);

mat3 calculate_snake_from_skin(in vec2 circle_coord) {
	float ang1 = asin(circle_coord.y);
	float width = abs(cos(ang1));
	float ang = acos(clamp(circle_coord.x/width, -1.0, 1.0));

	vec3 xdir = vec3(sin(ang), 0, -cos(ang));

	float h = sqrt(1. - circle_coord.x*circle_coord.x - circle_coord.y*circle_coord.y);

	vec3 shape_normal = vec3(circle_coord.x, -circle_coord.y, h);
	vec3 zdir = shape_normal;

	float ang2 = atan(h, circle_coord.y);
	vec3 ydir = vec3(0, -sin(ang2), -cos(ang2));

	return mat3(xdir, ydir, zdir);
}

mat3 calculate_world_from_skin(in vec3 across, in vec3 along, in vec2 circle_coord) {
	mat3 world_from_snake = mat3(across, along, vec3(0, 0, 1));
	mat3 world_from_skin = world_from_snake * calculate_snake_from_skin(circle_coord);
	return world_from_skin;
}

vec2 calculate_texture_coord_due_to_cap(in vec2 circle_coord) {
	float ang1 = asin(circle_coord.y);
	float width = abs(cos(ang1));
	float ang = acos(clamp(circle_coord.x/width, -1.0, 1.0));

	float len_around_x = ((ang / M_PI) - 0.5) * 2. * width;
	float len_around_y = ((acos(circle_coord.y) / M_PI) - 0.5) * 2. * 2.5 * M_PI;

	return vec2((len_around_x / 2.) * (max_a - min_a) + ((max_a - min_a) / 2.), (len_around_y / 2.) * density);
}

void main(void) {
	vec2 texture_coord_due_to_cap = calculate_texture_coord_due_to_cap(circle_coord);
	vec2 texture_coord_due_to_b = vec2(b * 0.001, b * density);
	vec2 texture_coord = texture_coord_due_to_cap + texture_coord_due_to_b;

	vec4 diffuse = texture2D(diffuse_map, texture_coord);

	vec3 bump_normal = (vec3(texture2D(normal_map, texture_coord)) * 2.0 - 1.0) * vec3(-1, -1, 1);

	// The length of the interpolated bump_normal can be used as an estimate for
	// the local variance in normals, and can be used to reduced specular aliasing.
	float local_variance = 1.0 / length(bump_normal);

	// Add something to z to lower the perceived total height of the bump map
	bump_normal = normalize(bump_normal + vec3(0, 0, 0.5));

	mat3 world_from_skin = calculate_world_from_skin(normalize(across_i), normalize(along_i), circle_coord);
	vec3 normal = world_from_skin * bump_normal;

	float h = sqrt(1. - circle_coord.x*circle_coord.x - circle_coord.y*circle_coord.y);
	vec3 w = vec3(world_coord[0], world_coord[1], h*2.5);
	vec3 light = normalize(vec3(0, 0, 3) - w);

	gl_FragColor =
		diffuse * ambient +
		directional_light(normal, light, diffuse, local_variance)
	;
}
