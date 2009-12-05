attribute vec2 snake_coord_in;
attribute vec3 across_in, along_in;

varying vec2 snake_coord, world_coord;
varying vec3 across_i, along_i;

void main(void) {
	snake_coord = snake_coord_in;
	world_coord = gl_Vertex;
	across_i = across_in;
	along_i = along_in;

	gl_Position = ftransform();
}
