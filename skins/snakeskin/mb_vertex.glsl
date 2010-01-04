varying vec2 world_coord;

void main(void) {
	world_coord = gl_Vertex;
	gl_Position = ftransform();
}
