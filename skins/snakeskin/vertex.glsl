#ifdef GL_ES
precision mediump float;
#endif

uniform mat3 transform;

attribute vec2 vertex;
attribute vec2 circle_coord_in;
attribute vec3 across_in, along_in;
attribute float b_in;

varying vec2 circle_coord, world_coord;
varying vec3 across_i, along_i;
varying float b;

void main(void) {
	circle_coord = circle_coord_in;
	world_coord = vertex;
	across_i = across_in;
	along_i = along_in;
	b = b_in;

	gl_Position = vec4((transform * vec3(vertex, 1.0)).xy, 0.0, 1.0);
}
