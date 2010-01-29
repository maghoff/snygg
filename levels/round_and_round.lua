-- Round and round!
-- Patience might be more valuable than skill...

function create_board()
	s = segment_sequence()
	s:push_back(arc(vec(0, 0), 100, 0 * math.pi, 2 * math.pi, 1))
	s:push_back(open_segment(arc(vec(0, 0), 80,  1 * math.pi, 2 * math.pi, 1)))
	s:push_back(open_segment(arc(vec(0, 0), 65,  0 * math.pi, 1 * math.pi, 1)))
	s:push_back(open_segment(arc(vec(0, 0), 50,  1 * math.pi, 2 * math.pi, 1)))
	s:push_back(open_segment(arc(vec(0, 0), 35,  0 * math.pi, 1 * math.pi, 1)))
	return s
end

