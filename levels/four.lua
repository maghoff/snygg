-- Four

require "util"

function create_board()
	local heap = util_heap()

	heap:box(-100, -50, 100, 50, 10)

	for y=-2, 2 do
		heap:circle(0, y * 20, 3)
	end
	for x=-4, 4 do
		if x ~= 0 then
			heap:circle(x * 20, 0, 3)
		end
	end

	return heap:to_segment()
end

i = 0

function get_starting_position()
	local pos = vec(10 + ((i % 10) - 5) * 20, 0)

	i = i + 1

	return pos
end
