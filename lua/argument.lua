
local M = {}

function M.Check(args, da, ddarg)
    for key, value in ipairs(args) do
        if da == value or ddarg == value then
            return key
        end
        ::continue::
    end
    return nil
end

function M.Get(args, da, ddarg)
    for key, value in ipairs(args) do
        if da == value or ddarg == value then
            return args[key + 1]
        end
        ::continue::
    end
end

return M
