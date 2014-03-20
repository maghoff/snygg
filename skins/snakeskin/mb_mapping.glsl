uniform sampler2D storedValue;
varying vec2 screen_coord, world_coord;
uniform float screen_width;

uniform vec4 ambient;

vec4 directional_light(vec3 normal, vec3 light, vec4 diffuse, float phong_exponent, float local_variance);
vec4 color_space_mapping(vec4 linear);

float sample(vec2 p) {
	float j = mod(ceil(p.x * screen_width), 4.);
	vec4 vals = texture2D(storedValue, (p - vec2(j / screen_width, 0)));
	return vals[int(j)];
}

float h(vec2 p) {
	return 1. - 1./sample(p);
}

void main(void) {
	const vec3 light_pos = vec3(0, 0, 3);
	const vec4 diffuse = vec4(0.7, 0, 0, 1);
	const float phong_exponent = 200.0;
	const float local_variance = 1.0;

	float val = sample(screen_coord);
	float alpha = smoothstep(1., 1.1, val);

	vec3 w = vec3(world_coord, h(screen_coord));
	vec3 light = normalize(light_pos - w);

	const float a = 3.0;
	const float s = 1.0/800.0;
	vec3 t = vec3(1, 0, a*(h(screen_coord + s*vec2(1, 0)) - h(screen_coord + s*vec2(-1, 0))));
	vec3 b = vec3(0, 1, a*(h(screen_coord + s*vec2(0, 1)) - h(screen_coord + s*vec2(0, -1))));
	vec3 normal = normalize(cross(t, b));

	vec4 color =
		diffuse * ambient +
		directional_light(normal, light, diffuse, phong_exponent, local_variance)
	;

	//color = vec4(normal, 1);

	gl_FragColor = color_space_mapping(vec4(color.rgb, alpha));
}
