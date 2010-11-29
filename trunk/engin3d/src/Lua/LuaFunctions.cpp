#include "LuaFunctions.h"

#include "../Lua/LuaManager.h"
#include "../Character/CharacterManager.h"


int CreatePatrol( float posX, float posY, float posZ, float speed, float angSpeed )// C++ Foo
{
  cCharacter* lpCharacter = cCharacterManager::Get().CreateCharacter();
  lpCharacter->SetSpeed( speed );
  lpCharacter->SetAngSpeed( angSpeed );

  cBehaviourBase* lpBehaviour = cBehaviourManager::Get().CreateBehaviour( eCHASER_WITH_ORIENTATION );
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
	assert( liArgCount == 5 );

	// take the arguments from the stack
	//( use luaL_checkinteger for ints, luaL_checknumber for floats  )
	float lfArg1 = (float) luaL_checknumber( lpLuaContext, 1);
	float lfArg2 = (float) luaL_checknumber( lpLuaContext, 2);
	float lfArg3 = (float) luaL_checknumber( lpLuaContext, 3);
	float lfArg4 = (float) luaL_checknumber( lpLuaContext, 4);
	float lfArg5 = (float) luaL_checknumber( lpLuaContext, 5);

	// Insert Here the code to Do something...
	int liRet = CreatePatrol( lfArg1, lfArg2, lfArg3, lfArg4, lfArg5 );

	// Introducing the result in the Stack
	lua_pushinteger( lpLuaContext, liRet );

	// sending back the numbers of return´s values in the stack
	return 1;
}

void RegisterLuaFunctions() // C++ Foo Register in Lua
{
	cLuaManager::Get().Register( "CreatePatrol", CreatePatrol );
}