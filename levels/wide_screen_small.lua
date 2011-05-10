-- 16:9 YouTube-friendly board
-- Now smaller, to make details bigger on screen

require "util"

function create_board()
	local heap = util_heap()
	heap:box(-65, -35 - 10, 65, 35 - 10, 10)
	return heap
end
