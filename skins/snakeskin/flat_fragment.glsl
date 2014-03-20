#ifdef GL_ES
precision mediump float;
#endif

varying vec2 screen_coord, world_coord;

uniform vec4 ambient;
uniform vec4 diffuse;

vec4 directional_light(vec3 normal, vec3 light, vec4 diffuse, float phong_exponent, float local_variance);
vec4 color_space_mapping(vec4 linear);

void main(void) {
	const vec3 light_pos = vec3(0, 0, 3);
	const vec3 normal = vec3(0, 0, 1);
	const float phong_exponent = 1.0;
	const float local_variance = 1.0;

	vec3 w = vec3(world_coord, 0);
	vec3 light = normalize(light_pos - w);

	vec4 color =
		diffuse * ambient +
		directional_light(normal, light, diffuse, phong_exponent, local_variance)
	;

	gl_FragColor = color_space_mapping(vec4(color.rgb, 1.0));
}
