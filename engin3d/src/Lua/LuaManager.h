#ifndef LUAMANAGER_H
#define LUAMANAGER_H

#include "..\Utility\Singleton.h"
#include <iostream>
#include <cassert>
extern "C"
{
	#include "Src/lua.h"
	#include "Src/lauxlib.h"
	#include "Src/lualib.h"
}

// extern C: tells to compiler that the library 
// must be compiled in C (and not C++)


class cLuaManager : public cSingleton<cLuaManager>
{
	public:
		friend class cSingleton<cLuaManager>;

		void Init();
		void Deinit();
		bool DoString( const char * lacStatement );
		bool DoFile( const char* lacFile );
		void Register( const char* lacFuncName, lua_CFunction lpFunc );

		inline lua_State* GetContext() { return mpLuaContext; }
		bool CheckError( int liError );
		
		template < class Z>
			bool CallLua( const char* lacFuncName, Z* lpRet );	
		template <class T, class Z>
			bool CallLua( const char* lacFuncName, T lArg1, Z* lpRet );
		template <class T,class U, class Z>
			bool CallLua( const char* lacFuncName, T lArg1, U lArg2, Z* lpRet );	
		template <class T,class U, class O, class Z>
			bool CallLua( const char* lacFuncName, T lArg1, U lArg2, O lArg3, Z* lpRet );

	protected:
		cLuaManager(); // Protected constructor
	
	private:
		lua_State* mpLuaContext;

		void Pop( unsigned int luiNValues );
		
		// internal function overloaded
		void PushArg( int liArg );
		void PushArg( long liArg );
		void PushArg( float lfArg );
		void PushArg( const char* lacArg );
		void PushArg( std::string lacArg );
		void PushArg( bool lbArg );

		void GetRetValue( int* liRet );
		void GetRetValue( float* lfRet );
		void GetRetValue( long* liRet );
		void GetRetValue( std::string* lacRet );
		void GetRetValue( const char* lacRet );
		void GetRetValue( bool* lbRet );

};
// All template functions must be DEFINED HERE in headers 
// overloaded functions with 1,2 & 3 arguments (and with/without return)

template < class Z>
bool cLuaManager::CallLua(const char *lacFuncName, Z *lpRet)
{
		assert( mpLuaContext );

	// 1. Introduce the function´s name into the stack
	lua_getglobal( mpLuaContext, lacFuncName );

	// Checking Function definition in Lua
	int lbExists = lua_isfunction( mpLuaContext, -1 );

	if ( !lbExists ) // If It isn´t defined in Lua !
	{
		// Delete the function name from the Lua´s stack
		Pop( 1 );
		printf( "Error: The Function %s doesn´t exist \n", lacFuncName );
		return false;
	}
	// else if the function it´s defined...
	
	// 2. We haven´t arguments here, so continue

	// 3. Get the return Value (if there is one)
	if ( lpRet )
	{
		// function call
		lua_call( mpLuaContext, 0, 1 );
		// Getting the result
		GetRetValue( lpRet );
		// Deleting return value from stack (if there is one)
		Pop( 1 );
	}
	else // Function doesn´t return any value
	{
		// Function calling
		lua_call( mpLuaContext, 0, 0 );
	}
	return true;
}

template <class T, class Z>
bool cLuaManager::CallLua(const char *lacFuncName, T lArg1, Z *lpRet)
{
	assert( mpLuaContext );

	// 1. Introduce the function´s name into the stack
	lua_getglobal( mpLuaContext, lacFuncName );

	// Checking Function definition in Lua
	int lbExists = lua_isfunction( mpLuaContext, -1 );

	if ( !lbExists ) // If It isn´t defined in Lua !
	{
		// Delete the function name from the Lua´s stack
		Pop( 1 );
		printf( "Error: The Function %s doesn´t exist \n", lacFuncName );
		return false;
	}
	// else if the function it´s defined...

	// 2. Introduce arguments into the stack
	PushArg(lArg1);

	// 3. Get the return Value (if there is one)
	if ( lpRet )
	{
		// function call
		lua_call( mpLuaContext, 1, 1 );
		// Getting the result
		GetRetValue( lpRet );
		// Deleting return value from stack (if there is one)
		Pop( 1 );
	}
	else // Function doesn´t return any value
	{
		// Function calling
		lua_call( mpLuaContext, 1, 0 );
	}

	return true;
}


template <class T,class U, class Z> 
bool cLuaManager::CallLua( const char* lacFuncName, T lArg1, U lArg2, Z* lpRet )
{
	assert( mpLuaContext );

	// 1. Introduce the function´s name into the stack
	lua_getglobal( mpLuaContext, lacFuncName );

	// Checking Function definition in Lua
	int lbExists = lua_isfunction( mpLuaContext, -1 );

	if ( !lbExists ) // If It isn´t defined in Lua !
	{
		// Delete the function name from the Lua´s stack
		Pop( 1 );
		printf( "Error: The Function %s doesn´t exist \n", lacFuncName );
		return false;
	}
	// else if the function it´s defined...

	// 2. Introduce arguments into the stack
	PushArg(lArg1);
	PushArg(lArg2);

	// 3. Get the return Value (if there is one)
	if ( lpRet )
	{
		// function call
		lua_call( mpLuaContext, 2, 1 );
		// Getting the result
		GetRetValue( lpRet );
		// Deleting return value from stack (if there is one)
		Pop( 1 );
	}
	else // Function doesn´t return any value
	{
		// Function calling
		lua_call( mpLuaContext, 2, 0 );
	}

	return true;
}
template < class T, class U, class O, class Z>
bool cLuaManager::CallLua(const char *lacFuncName, T lArg1, U lArg2, O lArg3, Z *lpRet)
{
	assert( mpLuaContext );

	// 1. Introduce the function´s name into the stack
	lua_getglobal( mpLuaContext, lacFuncName );

	// Checking Function definition in Lua
	int lbExists = lua_isfunction( mpLuaContext, -1 );

	if ( !lbExists ) // If It isn´t defined in Lua !
	{
		// Delete the function name from the Lua´s stack
		Pop( 1 );
		printf( "Error: The Function %s doesn´t exist \n", lacFuncName );
		return false;
	}
	// else if the function it´s defined...

	// 2. Introduce arguments into the stack
	PushArg(lArg1);
	PushArg(lArg2);
	PushArg(lArg3);

	// 3. Get the return Value (if there is one)
	if ( lpRet )
	{
		// function call
		lua_call( mpLuaContext, 3, 1 );
		// Getting the result
		GetRetValue( lpRet );
		// Deleting return value from stack (if there is one)
		Pop( 1 );
	}
	else // Function doesn´t return any value
	{
		// Function calling
		lua_call( mpLuaContext, 3, 0 );
	}

	return true;
}


#endif