-- Round and round!
-- Patience might be more valuable than skill...

require "util"

function create_board()
	local heap = util_heap()
	local pi = math.pi

	heap:circle(0, 0, 100)
	heap:arc(0, 0, 80, 1 * pi, 2 * pi)
	heap:arc(0, 0, 65, 0 * pi, 1 * pi)
	heap:arc(0, 0, 50, 1 * pi, 2 * pi)
	heap:arc(0, 0, 35, 0 * pi, 1 * pi)

	return heap:to_segment()
end

