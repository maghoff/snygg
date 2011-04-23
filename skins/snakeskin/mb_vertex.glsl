varying vec2 world_coord, screen_coord;

void main(void) {
	world_coord = gl_Vertex.xy;
	screen_coord = ftransform().xy * vec2(0.5, 0.5) - vec2(0.5, 0.5);
	gl_Position = ftransform();
}
