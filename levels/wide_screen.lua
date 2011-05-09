-- 16:9 YouTube-friendly board

require "util"

function create_board()
	local heap = util_heap()
	heap:box(-120, -65, 120, 65, 10)
	return heap:to_segment()
end
