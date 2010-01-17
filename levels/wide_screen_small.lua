-- 16:9 YouTube-friendly board
-- Now smaller, to make details bigger on screen

require "levels/util"

function create_board()
	return box(rect(-60, -35 - 10, 60, 35 - 10), 10)
end
