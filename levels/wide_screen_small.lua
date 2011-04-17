-- 16:9 YouTube-friendly board
-- Now smaller, to make details bigger on screen

require "util"

function create_board()
	return contour(box(rect(-65, -35 - 10, 65, 35 - 10), 10))
end
