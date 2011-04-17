-- Four

require "util"

function create_board()
	s = segment_sequence()
	s.circle = function(self, x, y, rad)
		self:push_back(contour(arc(vec(x, y), rad, 2 * math.pi, 0, 1)))
	end

	s:push_back(contour(box(rect(-100, -50, 100, 50), 10)))

	for y=-2, 2 do
		s:circle(0, y * 20, 3)
	end
	for x=-4, 4 do
		if x ~= 0 then
			s:circle(x * 20, 0, 3)
		end
	end
	return s
end

i = 0

function get_starting_position()
	local pos = vec(10 + ((i % 10) - 5) * 20, 0)

	i = i + 1

	return pos
end
