uniform sampler2D diffuse_map;
uniform sampler2D normal_map;

vec4 get_diffuse(vec2 texture_coord) {
	return texture2D(diffuse_map, texture_coord);
}

vec3 get_bump_normal(vec2 texture_coord) {
	return (vec3(texture2D(normal_map, texture_coord)) * 2.0 - 1.0) * vec3(-1, -1, 1);
}
