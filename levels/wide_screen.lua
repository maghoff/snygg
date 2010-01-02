-- 16:9 YouTube-friendly board

require "levels/util"

function create_board()
	return box(rect(-120, -65, 120, 65), 10)
end
