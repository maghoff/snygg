-- I saw a manhole cover (bekkalokk) that looked like this

require "util"

function create_board()
	local heap = util_heap()
	local tau = 2*math.pi

	local r = 100
	local line_len = 10
	local spacing = 50

	local bump_angular_size = tau/30
	local n_bumps = 3
	local thickness = 2.5
	local l = 10
	local start_ang = 0 + bump_angular_size/2

	local bigarc_size = tau/n_bumps - bump_angular_size
	local phi = tau/4 + bump_angular_size/2
	local bump_linear_size = 2*r*math.sin(bump_angular_size/2)

	local t = heap:turtle(r*math.cos(start_ang), r*math.sin(start_ang), -math.sin(start_ang), math.cos(start_ang))
	for i = 1, n_bumps do
		t.left(r, bigarc_size)
		t.left(thickness, phi)
		t.forward(l)
		t.right((bump_linear_size - 2*thickness)/2, tau/2)
		t.forward(l)
		t.left(thickness, phi)
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
