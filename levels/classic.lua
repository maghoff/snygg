-- Classic board

require "util"

function create_board()
	local heap = util_heap()
	heap:box(-200, -50, 200, 50, 10)
	return heap
end
