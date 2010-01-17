mat3 calculate_snake_from_skin(in vec2 circle_coord) {
	float ang1 = asin(circle_coord.y);
	float width = abs(cos(ang1));
	float ang = acos(clamp(circle_coord.x/width, -1.0, 1.0));

	vec3 xdir = vec3(sin(ang), 0, -cos(ang));

	float h = sqrt(1 - circle_coord.x*circle_coord.x - circle_coord.y*circle_coord.y);

	vec3 shape_normal = vec3(circle_coord.x, -circle_coord.y, h);
	vec3 zdir = shape_normal;

	float ang2 = atan2(h, circle_coord.y);
	vec3 ydir = vec3(0, -sin(ang2), -cos(ang2));

	return mat3(xdir, ydir, zdir);
}
