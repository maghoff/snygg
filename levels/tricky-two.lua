-- Obstacles inside the board

require "util"

function create_board()
	local heap = util_heap()

	local w, h, margin = 70, 70, 20

	-- Frame:
	frame = heap:turtle(-w, -h, 1, 0)
	frame.forward(2*w)
	frame.left(h, math.pi)
	frame.forward(2*w)
	frame.left(h, math.pi)

	-- Left inner circle:
	heap:circle(-w, 0, h-margin)

	-- Right inner circle:
	heap:circle( w, 0, h-margin)

	return heap
end

i = 0

function get_starting_position()
	local pos = vec(-10 + i * 20, -40)

	i = (i + 1) % 2

	return pos
end
