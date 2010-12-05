#include "LuaFunctions.h"

#include "../Lua/LuaManager.h"
#include "../Character/CharacterManager.h"
#include "../Graphics/GraphicManager.h"


int CreatePatrol( float posX, float posY, float posZ, float speed, float angSpeed, cVec3 colour)// C++ Foo
{
  cCharacter* lpCharacter = cCharacterManager::Get().CreateCharacter();
  lpCharacter->SetSpeed( speed );
  lpCharacter->SetAngSpeed( angSpeed );
  lpCharacter->SetColour( colour );

  cBehaviourBase* lpBehaviour = cBehaviourManager::Get().CreateBehaviour( ePATROL );
  lpCharacter->SetActiveBehaviour( lpBehaviour, posX, posY, posZ );

	return lpCharacter->GetId() ;
}

int CreatePatrol( lua_State* lpLuaContext )// Foo for use C++ Foo in Lua
{
	// check the context is right
	assert( lpLuaContext );
	
	// Take the number of arguments from Lua Stack
	int liArgCount = lua_gettop( lpLuaContext );

	// Check if the number of Arg is right
	assert( liArgCount == 8 );

	// take the arguments from the stack
	//( use luaL_checkinteger for ints, luaL_checknumber for floats  )
	float lfArg1 = (float) luaL_checknumber( lpLuaContext, 1);
	float lfArg2 = (float) luaL_checknumber( lpLuaContext, 2);
	float lfArg3 = (float) luaL_checknumber( lpLuaContext, 3);
	float lfArg4 = (float) luaL_checknumber( lpLuaContext, 4);
	float lfArg5 = (float) luaL_checknumber( lpLuaContext, 5);
  float lfArg6 = (float) luaL_checknumber( lpLuaContext, 6);
  float lfArg7 = (float) luaL_checknumber( lpLuaContext, 7);
	float lfArg8 = (float) luaL_checknumber( lpLuaContext, 8);

	// Insert Here the code to Do something...
	int liRet = CreatePatrol( lfArg1, lfArg2, lfArg3, lfArg4, lfArg5, cVec3(lfArg6, lfArg7, lfArg8) );

	// Introducing the result in the Stack
	lua_pushinteger( lpLuaContext, liRet );

	// sending back the numbers of return´s values in the stack
	return liRet;
}

//Set the new target 
void SetPatrolTarget( int miCharacterId, float mfNextPosX, float mfNextPosY, float mfNextPosZ) 
{
  cCharacter * lcCharacter = cCharacterManager::Get().SearchCharacter( miCharacterId );
  if (lcCharacter)
  {
   cBehaviourBase * lcBehaviour = lcCharacter->GetActiveBehaviour();
   if (lcBehaviour)
     lcBehaviour->SetTarget(mfNextPosX, mfNextPosY, mfNextPosZ);
  }
}

//Set patrol target glue method
int SetPatrolTarget( lua_State* lpLuaContext )
{
	// check the context is right
	assert( lpLuaContext );
	
	// Take the number of arguments from Lua Stack
	int liArgCount = lua_gettop( lpLuaContext );

	// Check if the number of Arg is right
	assert( liArgCount == 4 );

	// take the arguments from the stack
	//( use luaL_checkinteger for ints, luaL_checknumber for floats  )
	int liArg1 = (int) luaL_checkinteger( lpLuaContext, 1);
	float lfArg2 = (float) luaL_checknumber( lpLuaContext, 2);
	float lfArg3 = (float) luaL_checknumber( lpLuaContext, 3);
	float lfArg4 = (float) luaL_checknumber( lpLuaContext, 4);
	
	// Call to set patrol
  SetPatrolTarget( liArg1, lfArg2, lfArg3, lfArg4 );

 	// Introducing the result in the Stack
	lua_pushinteger( lpLuaContext, 1 );

  //return 1
	return 1;
}

//Set patrol target glue method
int DrawLine( lua_State* lpLuaContext )
{
	// check the context is right
	assert( lpLuaContext );
	
	// Take the number of arguments from Lua Stack
	int liArgCount = lua_gettop( lpLuaContext );

	// Check if the number of Arg is right
	assert( liArgCount == 9 );

	// take the arguments from the stack
	//( use luaL_checkinteger for ints, luaL_checknumber for floats  )
	float lfArg1 = (float) luaL_checknumber( lpLuaContext, 1);
	float lfArg2 = (float) luaL_checknumber( lpLuaContext, 2);
	float lfArg3 = (float) luaL_checknumber( lpLuaContext, 3);
	float lfArg4 = (float) luaL_checknumber( lpLuaContext, 4);
  float lfArg5 = (float) luaL_checknumber( lpLuaContext, 5);
  float lfArg6 = (float) luaL_checknumber( lpLuaContext, 6);
  float lfArg7 = (float) luaL_checknumber( lpLuaContext, 7);
	float lfArg8 = (float) luaL_checknumber( lpLuaContext, 8);
  float lfArg9 = (float) luaL_checknumber( lpLuaContext, 9);

	// Draw Line
  cGraphicManager::Get().DrawLine(cVec3( lfArg1, lfArg2, lfArg3), 
                                  cVec3( lfArg4, lfArg5, lfArg6), 
                                  cVec3( lfArg7, lfArg8, lfArg9));
	
 	// Introducing the result in the Stack
	lua_pushinteger( lpLuaContext, 1 );

  //return 1
	return 1;
}

int CreatePlayer( float posX, float posY, float posZ, float speed, float angSpeed, cVec3 colour)
{
  cCharacter* lpCharacter = cCharacterManager::Get().CreateCharacter();
  lpCharacter->SetSpeed( speed );
  lpCharacter->SetAngSpeed( angSpeed );
  lpCharacter->SetColour( colour );

  cBehaviourBase* lpBehaviour = cBehaviourManager::Get().CreateBehaviour( ePLAYER_CONTROLLER );
  lpCharacter->SetActiveBehaviour( lpBehaviour, posX, posY, posZ );

  lpCharacter->SetPosition(cVec3(posX, posY, posZ));
	return lpCharacter->GetId() ;
}

int CreatePlayer( lua_State* lpLuaContext )// Foo for use C++ Foo in Lua
{
	// check the context is right
	assert( lpLuaContext );
	
	// Take the number of arguments from Lua Stack
	int liArgCount = lua_gettop( lpLuaContext );

	// Check if the number of Arg is right
	assert( liArgCount == 8 );

	// take the arguments from the stack
	//( use luaL_checkinteger for ints, luaL_checknumber for floats  )
	float lfArg1 = (float) luaL_checknumber( lpLuaContext, 1);
	float lfArg2 = (float) luaL_checknumber( lpLuaContext, 2);
	float lfArg3 = (float) luaL_checknumber( lpLuaContext, 3);
	float lfArg4 = (float) luaL_checknumber( lpLuaContext, 4);
	float lfArg5 = (float) luaL_checknumber( lpLuaContext, 5);
  float lfArg6 = (float) luaL_checknumber( lpLuaContext, 6);
  float lfArg7 = (float) luaL_checknumber( lpLuaContext, 7);
	float lfArg8 = (float) luaL_checknumber( lpLuaContext, 8);

	// Insert Here the code to Do something...
	int liRet = CreatePlayer( lfArg1, lfArg2, lfArg3, lfArg4, lfArg5, cVec3(lfArg6, lfArg7, lfArg8) );

	// Introducing the result in the Stack
	lua_pushinteger( lpLuaContext, liRet );

	// sending back the numbers of return´s values in the stack
	return liRet;
}


void RegisterLuaFunctions() // C++ Foo Register in Lua
{
	cLuaManager::Get().Register( "CreatePatrol", CreatePatrol );
  cLuaManager::Get().Register( "CreatePlayer", CreatePlayer );
  cLuaManager::Get().Register( "SetPatrolTarget", SetPatrolTarget );
  cLuaManager::Get().Register( "DrawLine", DrawLine );
}

