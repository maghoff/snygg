-- Obstacles inside the board

require "util"

function create_board()
	s = segment_sequence()

	-- Frame:
	s:push_back(box(rect(-120, -65, 120, 65), 30))

	-- Left inner box:
	s:push_back(box(rect(-100, -45, -40, 45), 10))

	-- Right inner box:
	s:push_back(box(rect(  40, -45, 100, 45), 10))

	return s
end
