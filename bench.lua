local fw = require("fastwalk")

local N = 1e6
local t = {}
for i = 1, N do t[i] = i end

local clock = os.clock
local function bench(name, f)
    local a = clock(); local r = f(); local b = clock()
    print(string.format("%-10s %.4fs  -> %.0f", name, b - a, r))
end

-- Baseline iterators
bench("pairs",   function() local s=0 for _,v in pairs(t) do s=s+v end return s end)
bench("ipairs",  function() local s=0 for _,v in ipairs(t) do s=s+v end return s end)

-- Indexed Lua loop (often the real fastest pure-Lua path for arrays)
bench("lua_fold", function()
    local s = 0
    for i = 1, N do s = s + t[i] end
    return s
end)

-- C-side reductions (one boundary crossing for the whole traversal)
bench("fatsum",  function() return fw.sum_values(t) end)
bench("count",   function() return fw.count(t) end)
bench("max",     function() return fw.max_value(t) end)
bench("min",     function() return fw.min_value(t) end)x_value(t) end)