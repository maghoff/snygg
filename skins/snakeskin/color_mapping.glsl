uniform vec4 color;

vec4 get_diffuse(vec2 texture_coord) {
	return color;
}

vec3 get_bump_normal(vec2 texture_coord) {
	return vec3(0, 0, 1);
}
