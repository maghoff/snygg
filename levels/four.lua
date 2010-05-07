-- Four

require "util"

function create_board()
	shift_x = 10
	s = segment_sequence()
	s:push_back(box(rect(-100 + shift_x, -50, 100 + shift_x, 50), 10))
	for y=-2, 2 do
		s:push_back(arc(vec(shift_x, y * 20), 3, 2 * math.pi, 0, 1))
	end
	for x=-4, 4 do
		s:push_back(arc(vec(shift_x + x * 20, 0), 3, 2 * math.pi, 0, 1))
	end
	return s
end
