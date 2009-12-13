-- Extremists board

function create_board()
    s = segment_sequence()
    s:push_back(box(rect(-24, -48, 24, 0), 10))
    return s
end
