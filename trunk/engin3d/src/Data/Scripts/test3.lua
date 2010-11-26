-- REQUIRES --

-- GLOBAL VARIABLES DECLARATION --
arg1 = 10
arg2 = 2

-- FUNCTIONS DEFINITION --

-- LUA CODE --
value = MultiplyFromC( arg1, arg2 )
print( arg1 .. " * " .. arg2 .. " = " .. value)