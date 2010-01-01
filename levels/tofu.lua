-- Eat food inside a block of tofu

-- This level exposes a weakness with the calculation of the bounding box

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

function create_board()
	local s = segment_sequence()

	local corner_radius = 10

	local pinch = 0.3

	local height = 90
	local width = 160

	-- Inner dimensions:
	local left = 0 - width/2
	local right = left + width
	local bottom = 0 - height/2 - 10
	local top = bottom + height

	local A = vec(left, bottom)
	local B = vec(right, bottom)
	local C = vec(right, top)
	local D = vec(left, top)

	s:push_back(arc(A, corner_radius, 2/2 * math.pi + pinch, 3/2 * math.pi + pinch, 1))
	s:push_back(bent_line(A, B, corner_radius, pinch))
	s:push_back(arc(B, corner_radius, 3/2 * math.pi - pinch, 4/2 * math.pi - pinch, 1))
	s:push_back(bent_line(B, C, corner_radius, -pinch))
	s:push_back(arc(C, corner_radius, 0/2 * math.pi + pinch, 1/2 * math.pi + pinch, 1))
	s:push_back(bent_line(C, D, corner_radius, pinch))
	s:push_back(arc(D, corner_radius, 1/2 * math.pi - pinch, 2/2 * math.pi - pinch, 1))
	s:push_back(bent_line(D, A, corner_radius, -pinch))

	return s
end
