-- REQUIRES --
require "./Data/Scripts/test2"

-- GLOBAL VARIABLES DECLARATION --
counter = 0

-- FUNCTIONS DEFINITION --

-- LUA CODE --
while ( counter < 10 ) do
	counter = Inc( counter )
	if ( counter % 2 == 0 ) then
		print( " Par: " .. counter )
	else 
		print( " Impar: " .. counter )
	end
end