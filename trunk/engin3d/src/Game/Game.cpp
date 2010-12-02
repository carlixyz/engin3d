#include "Game.h"

#include "../Lua/LuaManager.h"
#include "InputConfiguration.h"
#include "../Input/InputManager.h"
#include "../Character/CharacterManager.h"

#ifdef _WIN32
  #include <windows.h>
  #include "../Window/Window.h" 
#endif

//Method to initialize the game
bool cGame::Init()
{
  extern tActionMapping kaActionMapping[];
 
  //Init time and exit attribute
  mbFinish = false;
  mfTimeElapsed = 0.0f;

  //Init window
  cApplicationProperties mProperties;
  mProperties.macApplicationName = "Test 1";
  mProperties.mbFullscreen = false;
  mProperties.muiBits = 16;
  mProperties.muiWidth = 640 ;
  mProperties.muiHeight = 480;

  //Init Camera 
  m3DCamera.Init();
  float lfAspect = (float)mProperties.muiWidth/(float)mProperties.muiHeight;
  m3DCamera.SetPerspective(45.0f, lfAspect, 0.1f, 100.0f);
  m3DCamera.SetLookAt( cVec3(0.001f, 10.0f, 0.0f), cVec3(0.0f, 0.0f, 0.0f) );

  // Window Creation
  bool lbResult =  cWindow::Get().Init( mProperties)  ;

   //Init Input Manager
  cInputManager::Get().Init(kaActionMapping, eIA_Count);

  //Init OpenGL
  if (lbResult)
  {
    lbResult = cGraphicManager::Get().Init( &cWindow::Get() );

    //If something failed kill the window
    if (!lbResult)
      cWindow::Get().Deinit();
  }
  // Character Manager initialization
  cCharacterManager::Get().Init();

   // Behaviour Setting
  cBehaviourManager::Get().Init() ;

	// Init LuaManager
  cLuaManager::Get().Init();
  RegisterLuaFunctions(); // C++ Functions Registered into Lua

  // REMEMBER THE FOLDERS STRUCTURE TO ACCESS THE FILES 
  cLuaManager::Get().DoFile( "Src/Data/Scripts/CreatePatrol.lua"  );
//  cLuaManager::Get().DoString( "CreatePatrol( 5, 0, 5.5, 2.1, 3)" );

  return lbResult;
}

// Method to update the game
void cGame::Update(float lfTimeStep)
{
  cWindow::Get().Update();

  //Update Input manager
  cInputManager::Get().Update(lfTimeStep);

  // Character Update
  cCharacterManager::Get().Update(lfTimeStep);

  //Update time
  mfTimeElapsed += lfTimeStep;
  
	mbFinish = mbFinish || 
             cWindow::Get().GetCloseApplication() ||
             IsPressed(eIA_CloseApplication);
	if (mbFinish)
	{ 
		return;
	}
}

//Method to render the game 
void cGame::Render()
{
  //Clear buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  //Activate the 3D Camera
  cGraphicManager::Get().ActivateCamera( &m3DCamera );

  // Set the world matrix
  cMatrix lWorld;
  lWorld.LoadIdentity();

	lWorld.LoadTranslation(cVec3(1.5, 0, -1.5) );
	cGraphicManager::Get().SetWorldMatrix(lWorld);
  
	//Render Debug Lines
	cGraphicManager::Get().DrawGrid();
	/*
	cGraphicManager::Get().DrawAxis(); 
	cGraphicManager::Get().DrawPoint(cVec3( 1.5f, 0.0f, 1.5f),cVec3( 1.0, 0.0, 1.0));
	cGraphicManager::Get().DrawLine(cVec3( -1.5f, 0.0f, -1.5f), cVec3( -1.5f, 0.0f, 1.5f), cVec3( 1.0f, 1.0f, 0.0f));
	*/

	// Character Rendering
	cCharacterManager::Get().Render();
	
	cGraphicManager::Get().SwapBuffer();
}

//Method to deinitialize the game
bool cGame::Deinit()
{
  //Deinit Input Manager
  cInputManager::Get().Deinit();

  //Deinit CharacterManager
  cCharacterManager::Get().Deinit();

  // Behaviour Setting
  cBehaviourManager::Get().Deinit() ;

  //Deinit Lua Manager
  cLuaManager::Get().Deinit();

  //Deinit graphic manager
  bool lbResult = cGraphicManager::Get().Deinit();

  //Deinit window
	lbResult =  lbResult && cWindow::Get().Deinit();
  return lbResult;
}
 