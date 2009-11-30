attribute vec2 snake_coord_in;

varying vec2 snake_coord;

void main(void) {
	gl_Position = ftransform();

	snake_coord = snake_coord_in;
}
