function bent_line(A, B, corner_radius, pinch)
	local AB, lr, e, f, ang, C, base_angle, pinch_radius

	AB = B + -1 * A

	lr = AB:length() / (2 * math.sin(pinch))
	pinch_radius = lr - corner_radius

	f = AB * (1 / AB:length())
	e = vec(y(f), -x(f))
	ang = math.cos(pinch) * e + math.sin(pinch) * f
	C = A + lr * ang

	base_angle = math.atan2(-y(e), -x(e))

	if pinch_radius < 0 then
		pinch_radius = -pinch_radius
		base_angle = math.pi + base_angle
	end

	return arc(
		C, pinch_radius,
		base_angle + pinch, base_angle - pinch,
		1
	)
end
