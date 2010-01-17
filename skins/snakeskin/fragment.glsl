uniform sampler2D diffuse_map;
uniform sampler2D normal_map;

varying vec2 snake_coord, world_coord;
varying vec3 across_i, along_i;

const float M_PI = 3.14159265358979;

const float density = 0.04;
const float min_a = 0.1, max_a = 0.45;

extern const vec4 ambient;
vec4 directional_light(vec3 normal, vec3 light, vec4 diffuse, float local_variance);

void main(void) {
	vec3 across = normalize(across_i);
	vec3 along = normalize(along_i);

	float ang = acos(clamp(snake_coord[0], -1.0, 1.0));
	float len_around = (ang / M_PI - 0.5) * 2;
	vec2 texture_coord = vec2(
		len_around/2 * (max_a - min_a) + ((max_a - min_a) / 2) +
		snake_coord[1] * 0.001
	,
		snake_coord[1] * density
	);

	vec4 diffuse = texture2D(diffuse_map, texture_coord);
	vec3 bump_normal = (vec3(texture2D(normal_map, texture_coord)) * 2.0 - 1.0) * vec3(-1, -1, 1);
	//bump_normal = normalize(vec3(0, 0, 1));

	// The length of the interpolated bump_normal can be used
	// as an estimate for the local variance in normals.
	float local_variance = 1.0 / length(bump_normal);

	bump_normal = normalize(bump_normal + vec3(0, 0, 0.5));

	float h = sqrt(1 - snake_coord[0]*snake_coord[0]);
	vec2 shape_normal = vec2(snake_coord[0], h);

	mat2x3 world_from_snake = mat2x3(across, vec3(0, 0, 1));

	vec3 out_v = world_from_snake * shape_normal;
	vec3 out_v_p = world_from_snake * vec2(shape_normal[1], -shape_normal[0]);

	mat3 world_from_skin = mat3(out_v_p, along, out_v);
	vec3 normal = world_from_skin * bump_normal;
	//vec3 normal = bump_normal[0] * out_v_p + bump_normal[1] * along + bump_normal[2] * out_v;
	//normal = vec4(shape_normal[0], shape_normal[1], 0, 0);

	vec3 w = vec3(world_coord[0], world_coord[1], h*2.5);

	vec3 light = normalize(vec3(0, 0, 3) - w);

	gl_FragColor =
		diffuse * ambient +
		directional_light(normal, light, diffuse, local_variance)
	;
}
