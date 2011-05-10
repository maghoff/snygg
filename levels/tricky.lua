-- Obstacles inside the board

require "util"

function create_board()
	local heap = util_heap()

	-- Frame:
	heap:box(-120, -65, 120, 65, 30)

	-- Left inner box:
	heap:box(-100, -45, -40, 45, 10)

	-- Right inner box:
	heap:box(  40, -45, 100, 45, 10)

	return heap
end

i = 0

function get_starting_position()
	local pos = vec(-10 + i * 20, -40)

	i = (i + 1) % 2

	return pos
end
