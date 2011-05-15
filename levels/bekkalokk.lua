-- I saw a manhole cover (bekkalokk) that looked like this

require "util"

function create_board()
	local heap = util_heap()
	local tau = 2*math.pi

	local r = 100
	local line_len = 10
	local spacing = 50

	local plain_circle = 1
	if plain_circle == 1 then
		heap:circle(0, 0, r)
	else
		local bump_size = tau/42
		local n_bumps = 3

		local thickness = 2.5
		local l = 7
		local bigarc_size = tau/n_bumps - bump_size
		local inner_r = r - thickness - l
		local inner_turn_angle = bump_size - 2 * thickness / (r - thickness)
		local inner_turn_r = 0

		local t = heap:turtle(0, -r, 1, 0)
		for i = 1, n_bumps do
			t.left(r, bigarc_size)
			t.left(thickness, tau/4)
			t.forward(l)
			t.right(7, tau/2-0.15)
			t.forward(l)
			t.left(thickness, tau/4)
		end
	end

	heap.inside_line = function(heap, x1, y1, x2, y2)
		if (x1*x1 + y1*y1) > (r*r) then return end
		if (x2*x2 + y2*y2) > (r*r) then return end
		heap:line(x1, y1, x2, y2)
	end

	local hll = line_len / 2
	for row = -r/spacing, r/spacing do
		for col = -r/spacing, r/spacing do
			heap:inside_line(col*spacing - hll, row*spacing, col*spacing + hll, row*spacing)
			heap:inside_line((col+0.5)*spacing, row*spacing - hll, (col+0.5)*spacing, row*spacing + hll)

			heap:inside_line((col+0.5)*spacing - hll, (row+0.5)*spacing, (col+0.5)*spacing + hll, (row+0.5)*spacing)
			heap:inside_line(col*spacing, (row+0.5)*spacing - hll, col*spacing, (row+0.5)*spacing + hll)
		end
	end

	return heap
end

function get_starting_position()
	return vec(0, -45)
end
