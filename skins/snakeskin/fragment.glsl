varying vec2 snake_coord;

void main(void) {
	gl_FragColor = vec4(snake_coord[0], -snake_coord[0], sin(snake_coord[1]), 1);
}
