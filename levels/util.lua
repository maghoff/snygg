function bent_line(A, B, corner_radius, pinch)
	local AB, lr, e, f, ang, C, base_angle, pinch_radius

	AB = B + -1 * A

	lr = AB:length() / (2 * math.sin(pinch))
	pinch_radius = lr - corner_radius

	f = AB * (1 / AB:length())
	e = vec(y(f), -x(f))
	ang = math.cos(pinch) * e + math.sin(pinch) * f
	C = A + lr * ang

	base_angle = math.atan2(-y(e), -x(e))

	if pinch_radius < 0 then
		pinch_radius = -pinch_radius
		base_angle = math.pi + base_angle
	end

	return arc(
		C, pinch_radius,
		base_angle + pinch, base_angle - pinch,
		1
	)
end

function box(rc, sr)
	local s = segment_sequence()
	local x1, y1, x2, y2, w, h, pi

	x1 = rc.x1
	y1 = rc.y1
	x2 = rc.x2
	y2 = rc.y2
	w = rc:width() - 2 * sr
	h = rc:height() - 2 * sr
	pi = math.pi

	s:push_back(line(vec(x1 + sr, y1     ), vec( 1,  0), w))
	s:push_back(arc (vec(x2 - sr, y1 + sr), sr, pi * -0.5, pi *  0.0, 1))
	s:push_back(line(vec(x2     , y1 + sr), vec( 0,  1), h))
	s:push_back(arc (vec(x2 - sr, y2 - sr), sr, pi *  0.0, pi *  0.5, 1))
	s:push_back(line(vec(x2 - sr, y2     ), vec(-1,  0), w))
	s:push_back(arc (vec(x1 + sr, y2 - sr), sr, pi *  0.5, pi *  1.0, 1))
	s:push_back(line(vec(x1     , y2 - sr), vec( 0, -1), h))
	s:push_back(arc (vec(x1 + sr, y1 + sr), sr, pi * -1.0, pi * -0.5, 1))

	return s
end

function util_heap()
	function turtle(heap, x, y, dx, dy)
		forward = function(len)
			x2, y2 = x + dx*len, y + dy*len
			heap:line(x, y, x2, y2)
			x, y = x2, y2
		end

		left = function(r, ang)
			nx, ny = -dy, dx
			if ang < 0 then nx, ny = -nx, -ny end
			cx, cy = x+nx*r, y+ny*r
			start_ang = math.atan2(-ny, -nx)
			end_ang = start_ang + ang
			heap:arc(cx, cy, r, start_ang, end_ang)
			nx2, ny2 = math.cos(end_ang), math.sin(end_ang)
			dx, dy = -ny2, nx2
			if ang < 0 then dx, dy = -dx, -dy end
			x, y = cx+nx2*r, cy+ny2*r
		end

		right = function(r, ang)
			left(r, -ang)
		end

		return {
			forward = forward,
			left = left,
			right = right
		}
	end

	heap = segment_heap()

	heap.turtle = turtle

	return heap
end
