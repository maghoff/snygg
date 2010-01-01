-- Eat food inside a block of tofu

-- This level exposes a weakness with the calculation of the bounding box

function bent_line(A, B, corner_radius, pinch)
	AB = B + -1 * A

	lr = AB:length() / (2 * math.sin(pinch))
	pinch_radius = lr - corner_radius

	f = AB * (1 / AB:length())
	e = vec(y(f), -x(f))
	ang = math.cos(pinch) * e + math.sin(pinch) * f
	C = A + lr * ang

	base_angle = math.atan2(-y(e), -x(e))

	return arc(
		C, pinch_radius,
		base_angle + pinch, base_angle - pinch,
		1
	)
end

function create_board()
	s = segment_sequence()

	corner_radius = 10

	pinch = 0.3

	height = 90
	width = 160

	-- Inner dimensions:
	left = 0 - width/2
	right = left + width
	bottom = 0 - height/2 - 10
	top = bottom + height


	s:push_back(arc(vec(left, bottom), corner_radius, 2/2 * math.pi, 3/2 * math.pi + pinch, 1))

	s:push_back(bent_line(vec(left, bottom), vec(right, bottom), corner_radius, pinch))

	s:push_back(arc(vec(right, bottom), corner_radius, 3/2 * math.pi - pinch, 4/2 * math.pi, 1))
	s:push_back(line(vec(right + corner_radius, bottom), vec(0, 1), height))
	s:push_back(arc(vec(right, top), corner_radius, 0/2 * math.pi, 1/2 * math.pi + pinch, 1))

	s:push_back(bent_line(vec(right, top), vec(left, top), corner_radius, pinch))

	s:push_back(arc(vec(left, top), corner_radius, 1/2 * math.pi - pinch, 2/2 * math.pi, 1))
	s:push_back(line(vec(left - corner_radius, top), vec(0, -1), height))

	return s
end
