-- Classic board

function create_board()
    s = segment_sequence()
    s:push_back(box(rect(-200, -50, 200, 50), 10))
    return s
end
