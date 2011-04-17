-- 16:9 YouTube-friendly board

require "util"

function create_board()
	return contour(box(rect(-120, -65, 120, 65), 10))
end
