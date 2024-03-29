-- REQUIRES --
-- GLOBAL VARIABLES DECLARATION --

posX = 0.0
posY = 0.0
posZ = 0.0
speed = 2.5
angSpeed = 3.0

playerX = -5.0
playerY = 0.0
playerZ = -5.0
playerSpeed = 5
playerAngSpeed = 3
playerId = 2
awareradius = 2.5

--Inicialize arrays
lPos = {}
lPos[1] = {}
-- lPos[2] = {}
-- lPos[3] = {}
-- lPos[4] = {}
--Patrol 1
lPos[1][1] = {x=0.0, y=0.0, z=-3.0}
lPos[1][2] = {x=3.0, y=0.0, z=0.0}
lPos[1][3] = {x=0.0, y=0.0, z=3.0}
lPos[1][4] = {x=-3.0, y=0.0, z=0.0}

--Patrol 2
-- lPos[2][1] = {x=3.0, y=0.0, z=3.0}
-- lPos[2][2] = {x=3.0, y=0.0, z=-3.0}
-- lPos[2][3] = {x=-3.0, y=0.0, z=-3.0}
-- lPos[2][4] = {x=-3.0, y=0.0, z=3.0}
-- lPos[2][5] = {x=0.0, y=0.0, z=5.0}

--Patrol 3
-- lPos[3][1] = {x=0.0, y=0.0, z=3.0}
-- lPos[3][2] = {x=-3.0, y=0.0, z=-3.0}
-- lPos[3][3] = {x=3.0, y=0.0, z=-3.0}


--Patrol 4
-- lPos[4][1] = {x=2.0, y=0.0, z=2.0}
-- lPos[4][2] = {x=5.0, y=0.0, z=-3.0}
-- lPos[4][3] = {x=-4.0, y=0.0, z=-1.0}
-- lPos[4][4] = {x=-3.0, y=0.0, z=4.0}
-- lPos[4][5] = {x=0.0, y=0.0, z=2.0}


--Current state array
lCurrentStates = {} 
lCurrentStates[1] = 0
-- lCurrentStates[2] = 0
-- lCurrentStates[3] = 0
-- lCurrentStates[4] = 0

--Colour path array
lColour = {}
lColour[1] = {r=1.0, g=0.35, b=0.35}
-- lColour[2] = {r=0.35, g=0.35, b=1.0}
-- lColour[3] = {r=0.35, g=1.0, b=0.35}
-- lColour[4] = {r=1.00, g=1.0, b=0.35}

-- FUNCTIONS DEFINITION --


function NextEndPoint( liCharacterId )
	if liCharacterId~=playerId then
		lPointCount = table.getn(lPos[liCharacterId])
		--Change to next state
		lNextState = (lCurrentStates[ liCharacterId ]%lPointCount) + 1 

		--Update state array
		lCurrentStates[ liCharacterId ] = lNextState

		--Update the next target point
		SetPatrolTarget( liCharacterId, lPos[liCharacterId][lNextState].x, lPos[liCharacterId][lNextState].y, lPos[liCharacterId][lNextState].z)
	end

end

function DrawPath( liCharacterId )
	if liCharacterId~=playerId then
		lPointCount = table.getn(lPos[liCharacterId])
		for i = 1,lPointCount do  
			i2 = (i%lPointCount)+1
	    
			DrawLine(lPos[liCharacterId][i].x, lPos[liCharacterId][i].y, lPos[liCharacterId][i].z,
			         lPos[liCharacterId][i2].x, lPos[liCharacterId][i2].y, lPos[liCharacterId][i2].z,
				     lColour[liCharacterId].r, lColour[liCharacterId].g, lColour[liCharacterId].b)
		end
	end
end

-- LUA CODE --


CreatePatrol(  posX, posY, posZ, speed, angSpeed , lColour[1].r, lColour[1].g, lColour[1].b, playerId, awareradius ) 
-- CreatePatrol(  posX, posY, posZ, speed, angSpeed , lColour[2].r, lColour[2].g, lColour[2].b, playerId, awareradius ) 
-- CreatePatrol(  posX, posY, posZ, speed, angSpeed , lColour[3].r, lColour[3].g, lColour[3].b, playerId, awareradius ) 
-- CreatePatrol(  posX, posY, posZ, speed, angSpeed , lColour[4].r, lColour[4].g, lColour[4].b, playerId, awareradius ) 
CreatePlayer(  playerX, playerY, playerZ, playerSpeed, playerAngSpeed , 1.0, 1.0, 1.0 ) 

