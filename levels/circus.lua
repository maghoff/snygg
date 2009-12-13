-- Family circus board

function create_board()
    s = segment_sequence()
    s:push_back(arc(vec(0, 0), 70, 0, 2 * math.pi, 1))
    return s
end
