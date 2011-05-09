-- Family circus board

require "util"

function create_board()
	local heap = util_heap()
	heap:circle(0, 0, 70)
	return heap:to_segment()
end
