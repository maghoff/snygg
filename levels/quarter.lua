-- Big quarter circle

require "util"

function create_board()
	local heap = util_heap()
	local pi = math.pi

	local sr = 10 -- small radius
	local br = 140 -- big radius

	heap:arc(0, -(br-sr), sr, 1.5 * pi, 2.0 * pi)
	heap:line(sr, -(br-sr), sr, 0)
	heap:arc(0, 0, sr, 0, 0.5 * pi)
	heap:line(0, sr, -(br-sr), sr)
	heap:arc(-br+sr, 0, sr, 0.5 * pi, 1.0 * pi)
	heap:arc(0, 0, br, 1.0 * pi, 1.5 * pi)

	return heap:to_segment()
end
