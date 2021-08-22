--debug = nil

--package.path = package.path .. ";?.lua"
local argument = require("argument")


function fact(n)
    if n == 0 then
        return 1
    else
        return n * fact(n - 1)
    end
end

function main()
    print("enter a number:")
    a = io.read("*number")
    print(fact(a))
end

--main()

-- triple comment, just remove first and it actually turns into comment lol
---[[
print(abcdefg) -- nil
--]]

print(package.path)

if arg then
    print(arg[0])
end

if argument.Check(arg, "-d", "--data") then
    print("DATA!")
end
if argument.Check(arg, "-i", "--interactive") then
    print("Why now?")
end
local v = argument.Get(arg, "-v", "--value")
if v then
    print(v)
end

print("Type of a type is", type(type("a")))
