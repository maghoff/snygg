-- 16:9 YouTube-friendly board

function create_board()
    s = segment_sequence()
    s:push_back(box(rect(-120, -65, 120, 65), 10))
    return s
end
