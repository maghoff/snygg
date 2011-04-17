-- Obstacles inside the board

require "util"

function create_board()
	s = segment_sequence()

	-- Frame:
	s:push_back(contour(box(rect(-120, -65, 120, 65), 30)))

	-- Left inner box:
	s:push_back(reverse_contour(box(rect(-100, -45, -40, 45), 10)))

	-- Right inner box:
	s:push_back(reverse_contour(box(rect(  40, -45, 100, 45), 10)))

	return s
end

i = 0

function get_starting_position()
	local pos = vec(-10 + i * 20, -40)

	i = (i + 1) % 2

	return pos
end
