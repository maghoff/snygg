-- Extremists board

require "util"

function create_board()
	local heap = util_heap()
	heap:box(-24, -48, 24, 0, 10)
	return heap
end
