-- Big quarter circle -- to demonstrate a problem in the calculation of the bounding box

function create_board()
    s = segment_sequence()

    sr = 10
    br = 140

    s:push_back(arc(vec(0, -(br-sr)), sr, 3/2 * math.pi, 2*math.pi, 1))
    s:push_back(line(vec(sr, -(br-sr)), vec(0, 1), br-sr))
    s:push_back(arc(vec(0, 0), sr, 0, (1/2)*math.pi, 1))
    s:push_back(line(vec(0, sr), vec(-1, 0), br-sr))
    s:push_back(arc(vec(-br+sr, 0), sr, 1/2 * math.pi, math.pi, 1))
    s:push_back(arc(vec(0, 0), br, math.pi, 3/2 * math.pi, 1))


    return s
end