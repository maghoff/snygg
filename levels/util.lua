function util_heap()
	local pi = math.pi

	local function turtle(heap, x, y, dx, dy)
		forward = function(len)
			local x2, y2 = x + dx*len, y + dy*len
			heap:line(x, y, x2, y2)
			x, y = x2, y2
		end

		left = function(r, ang)
			local nx, ny = -dy, dx
			if ang < 0 then nx, ny = -nx, -ny end
			local cx, cy = x+nx*r, y+ny*r
			local start_ang = math.atan2(-ny, -nx)
			local end_ang = start_ang + ang
			heap:arc(cx, cy, r, start_ang, end_ang)
			local nx2, ny2 = math.cos(end_ang), math.sin(end_ang)
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

	local function box(heap, x1, y1, x2, y2, sr)
		local w, h

		w = (x2-x1) - 2 * sr
		h = (y2-y1) - 2 * sr

		heap:line(x1 + sr, y1     , x2 - sr, y1     )
		heap:line(x1 + sr, y2     , x2 - sr, y2     )
		heap:line(x1     , y1 + sr, x1     , y2 - sr)
		heap:line(x2     , y1 + sr, x2     , y2 - sr)

		heap:arc(x2 - sr, y2 - sr, sr, pi * 0.0, pi * 0.5)
		heap:arc(x1 + sr, y2 - sr, sr, pi * 0.5, pi * 1.0)
		heap:arc(x1 + sr, y1 + sr, sr, pi * 1.0, pi * 1.5)
		heap:arc(x2 - sr, y1 + sr, sr, pi * 1.5, pi * 2.0)
	end

	local function circle(heap, x, y, r)
		heap:arc(x, y, r, 0, pi * 2.0)
	end

	local heap = segment_heap()

	heap.turtle = turtle
	heap.box = box
	heap.circle = circle

	return heap
end
