function os.capture(cmd, raw)
    local f = assert(io.popen(cmd, 'r'))
    local s = assert(f:read('*a'))
    f:close()
    if raw then return s end
    s = string.gsub(s, '^%s+', '')
    s = string.gsub(s, '%s+$', '')
    s = string.gsub(s, '[\n\r]+', ' ')
    return s
end


for i = 1, 12 do
    local cmd = "cmake-build-release/atomic_set_trail "..i
    local result = 0.0
    for j = 1, 4 do
        local s = os.capture(cmd)
        result = result + 0.25 * tonumber(s)
    end
    print(i.." "..result)
end