uniform sampler2D storedValue;
varying vec2 screen_coord, world_coord;

uniform vec4 ambient;

vec4 directional_light(vec3 normal, vec3 light, vec4 diffuse, float phong_exponent, float local_variance);

void main(void) {
	const vec3 light_pos = vec3(0, 0, 3);
	const vec3 normal = vec3(0, 0, 1);
	const vec4 diffuse = vec4(0.7, 0, 0, 1);
	const float phong_exponent = 7.0;
	const float local_variance = 1.0;

	float val = texture2D(storedValue, screen_coord).r;
	float alpha = smoothstep(1., 1.1, val);

	vec3 w = vec3(world_coord, 0);
	vec3 light = normalize(light_pos - w);

	vec4 color =
		diffuse * ambient +
		directional_light(normal, light, diffuse, phong_exponent, local_variance)
	;

	gl_FragColor = vec4(color.rgb, alpha);
}
