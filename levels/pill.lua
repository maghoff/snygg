-- Obstacles inside the board

require "util"

function create_board()
	local heap = util_heap()

	local w, h = 40, 60

	-- Frame, stolen from tricky-two:
	frame = heap:turtle(-w, -h, 1, 0)
	frame.forward(2*w)
	frame.left(h, math.pi)
	frame.forward(2*w)
	frame.left(h, math.pi)

	return heap
end

i = 0

function get_starting_position()
	local pos = vec(-10 + i * 20, -40)

	i = (i + 1) % 2

	return pos
end
