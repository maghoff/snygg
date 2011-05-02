-- Pacman

require "util"

SZ = 10

PACMAN_BOARD = {
	"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
	"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
	"XX............XX............XX",
	"XX.XXXX.XXXXX.XX.XXXXX.XXXX.XX",
	"XX*XXXX.XXXXX.XX.XXXXX.XXXX*XX",
	"XX.XXXX.XXXXX.XX.XXXXX.XXXX.XX",
	"XX..........................XX",
	"XX.XXXX.XX.XXXXXXXX.XX.XXXX.XX",
	"XX.XXXX.XX.XXXXXXXX.XX.XXXX.XX",
	"XX......XX....XX....XX......XX",
	"XXXXXXX.XXXXX XX XXXXX.XXXXXXX",
	"XXXXXXX.XXXXX XX XXXXX.XXXXXXX",
	"XXXXXXX.XX          XX.XXXXXXX",
	"XXXXXXX.XX XXXXXXXX XX.XXXXXXX",
	"XXXXXXX.XX XXXXXXXX XX.XXXXXXX",
	"       .   XXXXXXXX   .       ",
	"XXXXXXX.XX XXXXXXXX XX.XXXXXXX",
	"XXXXXXX.XX XXXXXXXX XX.XXXXXXX",
	"XXXXXXX.XX          XX.XXXXXXX",
	"XXXXXXX.XX XXXXXXXX XX.XXXXXXX",
	"XXXXXXX.XX XXXXXXXX XX.XXXXXXX",
	"XX............XX............XX",
	"XX.XXXX.XXXXX.XX.XXXXX.XXXX.XX",
	"XX.XXXX.XXXXX.XX.XXXXX.XXXX.XX",
	"XX*..XX.......  .......XX..*XX",
	"XXXX.XX.XX.XXXXXXXX.XX.XX.XXXX",
	"XXXX.XX.XX.XXXXXXXX.XX.XX.XXXX",
	"XX......XX....XX....XX......XX",
	"XX.XXXXXXXXXX.XX.XXXXXXXXXX.XX",
	"XX.XXXXXXXXXX.XX.XXXXXXXXXX.XX",
	"XX..........................XX",
	"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
	"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
}

function is_open(x, y)
	if y <= 0 or y > #PACMAN_BOARD then return false end
	if x <= 0 or x > #PACMAN_BOARD[y] then return false end
	return PACMAN_BOARD[y]:sub(x, x) ~= "X"
end

function consider_square(heap, ox, oy, x, y)
	if is_open(x, y) then return end

	lx = ox + (x-1) * SZ
	hy = oy - (y-1) * SZ
	hx = lx + SZ
	ly = hy - SZ
	cx = (lx + hx) / 2
	cy = (ly + hy) / 2

	n_opens = 0
	if is_open(x-1, y) then n_opens = n_opens + 1 end
	if is_open(x+1, y) then n_opens = n_opens + 1 end
	if is_open(x, y-1) then n_opens = n_opens + 1 end
	if is_open(x, y+1) then n_opens = n_opens + 1 end

	if n_opens == 1 then
		if is_open(x-1, y) or is_open(x+1, y) then
			heap:line(cx, ly, cx, hy)
		else
			heap:line(lx, cy, hx, cy)
		end
	elseif n_opens == 2 then
		b = 0

		arc_x = lx
		if is_open(x-1, y) then
			arc_x = hx
			b = b + math.pi * 0.5
		end

		arc_y = ly
		if is_open(x, y+1) then
			arc_y = hy
			b = math.pi * 3/2 - b
		end

		heap:arc(arc_x, arc_y, SZ/2, b, b + math.pi * 0.5)
	elseif n_opens == 0 then
		if is_open(x-1, y-1) or is_open(x-1, y+1) or is_open(x+1, y-1) or is_open(x+1, y+1) then
			b = 0

			arc_x = lx
			if is_open(x+1, y-1) or is_open(x+1, y+1) then
				arc_x = hx
				b = b + math.pi * 0.5
			end

			arc_y = ly
			if is_open(x-1, y-1) or is_open(x+1, y-1) then
				arc_y = hy
				b = math.pi * 3/2 - b
			end

			heap:arc(arc_x, arc_y, SZ/2, b, b + math.pi * 0.5)
		end
	end
end

function create_board()
	h = # PACMAN_BOARD
	w = # PACMAN_BOARD[1]

	ox = -0.5 * SZ * w
	oy =  0.5 * SZ * h

	s =	segment_sequence()
	s:push_back(contour(box(rect(ox-20, -oy-20, -ox+20, oy+20), 10)))

	heap = segment_heap()

	for y = 1, h do
		for x = 1, w do
			consider_square(heap, ox, oy, x, y)
		end
	end

	s:push_back(heap:to_segment())

	return s
end

function get_starting_position()
	return vec(0, -SZ * 8.5)
end
