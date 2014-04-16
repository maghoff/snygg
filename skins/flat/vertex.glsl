#ifdef GL_ES
precision mediump float;
#endif

uniform mat3 transform;

attribute vec2 vertex;

void main(void) {
	gl_Position = vec4((transform * vec3(vertex, 1.0)).xy, 0.0, 1.0);
}
