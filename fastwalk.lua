-- Load the compiled module without invoking this file again.
local path = package.path
package.path = ""
local fw = require("fastwalk")
package.path = path

--- Mean of all numeric values. Returns 0 for an empty table.
function fw.avg(t)
    local n = fw.count(t)
    return n == 0 and 0 or fw.sum_values(t) / n
end

return fw
