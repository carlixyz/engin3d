#include "LuaManager.h"

#ifdef _WIN32
 #include <Windows.h>
 #include <sstream>
 
void DbgOutInt(std::string label, std::string value )
 {
	 // Foo for check up in somewhere the Debug Output
	 std::stringstream strs;
	 strs << value << std::endl << std::endl;
	label.append(strs.str()) ;
	const char *c_str =label.c_str() ;
	OutputDebugString( c_str ) ;
 }
#endif

// Constructor
cLuaManager::cLuaManager()
{
	mpLuaContext = NULL; // setting a void pointer
}

void cLuaManager::Init()
{
	// create the Lua context
	mpLuaContext = lua_open();

	// Check the correct creation of the context
	assert( mpLuaContext );

	// Loading the Lua libraries
	luaL_openlibs( mpLuaContext );

}

bool cLuaManager::DoString(const char *lacStatement)
{
	assert( lacStatement );
	assert( mpLuaContext );
	int liRet = luaL_dostring( mpLuaContext, lacStatement );
	// Checking errors
	return CheckError( liRet );
}

bool cLuaManager::DoFile( const char* lacFile )
{
	assert( lacFile );
	assert( mpLuaContext );
	// Loading the script file
	int liRet = luaL_dofile( mpLuaContext, lacFile );
	// Checking errors
	return CheckError( liRet );
}

void cLuaManager::Register( const char* lacFuncName, lua_CFunction lpFunc )
{
	// Comprobamos que los argumentos no son NULL
	assert( mpLuaContext );
	assert( lpFunc );
	// Publicamos la función en Lua
	lua_register( mpLuaContext, lacFuncName, lpFunc );
}

void cLuaManager::Pop(unsigned int luiNValues)
{
	assert( mpLuaContext );
	lua_pop( mpLuaContext, (int) luiNValues );
}

bool cLuaManager::CheckError( int liError )
{
	// If liError it´s diferent of zero there is an error
	if ( liError != 0 )
	{
		assert( mpLuaContext );
		// Showing the error message (could be by outputdebug string)
	
		#ifdef _WIN32
		DbgOutInt("\n Error: \n",lua_tostring( mpLuaContext, -1 )  );
		#else	
		printf( "\n Error: %s\n", lua_tostring( mpLuaContext, -1 ) );
		#endif

		// taking the message from the top of the stack
		lua_pop( mpLuaContext, 1 );
		return false;
	}
	return true;
}

void cLuaManager::Deinit()
{
	if ( mpLuaContext )
	{
		// close the Lua context
		lua_close( mpLuaContext );
	}
	mpLuaContext = NULL;
}

// PushArg Overloads

void cLuaManager::PushArg(int liArg)
{
	assert( mpLuaContext );
	lua_pushinteger( mpLuaContext, liArg );
}

void cLuaManager::PushArg(long liArg)
{
	assert( mpLuaContext );
	lua_pushnumber( mpLuaContext, liArg );
}
void cLuaManager::PushArg(float lfArg)
{
	assert( mpLuaContext );
	lua_pushnumber( mpLuaContext, lfArg );
}
void cLuaManager::PushArg( const char *lacArg ) 
{
	assert( mpLuaContext );
	assert( lacArg );
	lua_pushstring( mpLuaContext, lacArg );
}
void cLuaManager::PushArg(std::string lacArg)
{
	assert( mpLuaContext );
	lua_pushstring( mpLuaContext, lacArg.c_str() );
}
void cLuaManager::PushArg(bool lbArg)
{
	assert( mpLuaContext );
	lua_pushboolean( mpLuaContext, lbArg );
}

// GetRetValue Overloads

void cLuaManager::GetRetValue(int *liRet)
{
	assert( mpLuaContext );
	*liRet = (int)lua_tointeger( mpLuaContext, -1 );
}
void cLuaManager::GetRetValue(float *lfRet)
{
	assert( mpLuaContext );
	*lfRet = (float)lua_tonumber( mpLuaContext, -1 );
}
void cLuaManager::GetRetValue(long *liRet)
{
	assert( mpLuaContext );
	*liRet = (long)lua_tonumber( mpLuaContext, -1 );
}
void cLuaManager::GetRetValue(std::string *lacRet)
{
	assert( mpLuaContext );
	*lacRet = (const char*)lua_tostring( mpLuaContext, -1 );
}
void cLuaManager::GetRetValue(const char *lacRet)
{
	assert( mpLuaContext );
	lacRet = lua_tostring( mpLuaContext, -1 );
}
void cLuaManager::GetRetValue(bool *lbRet)
{
	assert( mpLuaContext );
	*lbRet = ( lua_toboolean( mpLuaContext, -1 ) != 0);
}