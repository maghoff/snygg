-- Classic board

require "util"

function create_board()
	return contour(box(rect(-200, -50, 200, 50), 10))
end
