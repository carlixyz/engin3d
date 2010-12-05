-- REQUIRES --
-- GLOBAL VARIABLES DECLARATION --

posX = 0.0
posY = 0.0
posZ = 0.0
speed = 2.0
angSpeed = 1.5

--Inicialize arrays
lPos = {}
lPos[1] = {}
lPos[2] = {}
--Patrol 1
lPos[1][1] = {x=0.0, y=0.0, z=-3.0}
lPos[1][2] = {x=3.0, y=0.0, z=0.0}
lPos[1][3] = {x=0.0, y=0.0, z=3.0}
lPos[1][4] = {x=-3.0, y=0.0, z=0.0}

--Patrol 2
lPos[2][1] = {x=1.0, y=0.0, z=1.0}
lPos[2][2] = {x=1.0, y=0.0, z=-1.0}
lPos[2][3] = {x=-1.0, y=0.0, z=-1.0}
lPos[2][4] = {x=-1.0, y=0.0, z=1.0}


--Current state array
lCurrentStates = {} 
lCurrentStates[1] = 0
lCurrentStates[2] = 0

--Colour path array
lColour = {}
lColour[1] = {r=1.0, g=0.35, b=0.35}
lColour[2] = {r=0.4, g=0.4, b=0.8}
--Number of states
lPointCount = 4

-- FUNCTIONS DEFINITION --


function NextEndPoint( liCharacterId )
	--Change to next state
	lNextState = lCurrentStates[ liCharacterId ] + 1
	
	--If its the last move to the first
	if ( lNextState > lPointCount ) then
		lNextState = 1
	end

	--Update state array
	lCurrentStates[ liCharacterId ] = lNextState

	--Update the next target point
	SetPatrolTarget( liCharacterId, lPos[liCharacterId][lNextState].x, lPos[liCharacterId][lNextState].y, lPos[liCharacterId][lNextState].z)

end

function DrawPath( liCharacterId )
	for i = 1,lPointCount do  
		i2 = (i%lPointCount)+1
	    
		DrawLine(lPos[liCharacterId][i].x, lPos[liCharacterId][i].y, lPos[liCharacterId][i].z,
		         lPos[liCharacterId][i2].x, lPos[liCharacterId][i2].y, lPos[liCharacterId][i2].z,
		         lColour[liCharacterId].r, lColour[liCharacterId].g, lColour[liCharacterId].b)
	end
end

-- LUA CODE --

CreatePatrol(  posX, posY, posZ, speed, angSpeed ) 

