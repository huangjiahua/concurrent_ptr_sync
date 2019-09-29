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

local info = {
    cmd = "cmake-build-release/mrsw_ptr --operations 20000000 --read %f --onlytp true --hashsize %d --thread 4",
    tableSizes = {1, 10, 100, 1000, 10000, 100000, 1000000},
    threads = {1, 2, 4, 8},
    readRatios = {0.0, 0.2, 0.4, 0.6, 0.8, 0.9, 0.95, 1.0}
}


local output = ""
    
for _, i in pairs(info.tableSizes) do
    for _, j in pairs(info.readRatios) do
        local cmd = string.format(info.cmd, j, i)
        print(cmd)
        local captured = os.capture(cmd)
        output = output..captured.." "
    end
    output = output.."\n"
end

print(output)