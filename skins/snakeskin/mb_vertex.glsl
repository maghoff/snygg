#ifdef GL_ES
precision mediump float;
#endif

uniform mat3 transform;

attribute vec2 vertex;

varying vec2 world_coord, screen_coord;

void main(void) {
	world_coord = vertex;
	vec2 transformed = (transform * vec3(vertex, 1.0)).xy;
	screen_coord = transformed * vec2(0.5, 0.5) - vec2(0.5, 0.5);
	gl_Position = vec4(transformed, 0.0, 1.0);
}
