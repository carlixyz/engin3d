#include "Game.h"

#include "../Lua/LuaManager.h"
#include "InputConfiguration.h"
#include "../Input/InputManager.h"
#include "../Character/CharacterManager.h"
#include "../Graphics/Textures/TextureManager.h"
#include "../Gameplay/Scene/SceneManager.h"
#include "../Gameplay/Scene/Scene.h"

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
//  mfTimeElapsed = 0.0f; // not Needed

  //Init window
  cApplicationProperties mProperties;
  mProperties.macApplicationName = "Test 1";
  mProperties.mbFullscreen = false;
  mProperties.muiBits = 16;
  mProperties.muiWidth = 640 ;
  mProperties.muiHeight = 480;

  //Init Camera  

	// 3D Camera	
  m3DCamera.Init();
  float lfAspect = (float)mProperties.muiWidth/(float)mProperties.muiHeight;
  m3DCamera.SetPerspective(45.0f, lfAspect, 0.1f, 100.0f);
//  m3DCamera.SetLookAt( cVec3(0.001f, 10.0f, 0.0f), cVec3(0.0f, 0.0f, 0.0f) ); // Camera Used for IA
  m3DCamera.SetLookAt( cVec3(5.0f, 5.f, 5.f), cVec3(0.0f, 0.0f, 0.0f) );		// Camera Used for Jesus "+"


  	// * 2D Camera * // Uncomment this for IA
  float lfRight = (float) mProperties.muiWidth / 2.0f;
  float lfLeft = -lfRight;
  float lfTop = (float) mProperties.muiHeight / 2.0f;
  float lfBottom = -lfTop;
  m2DCamera.Init();
  m2DCamera.SetOrtho(lfLeft,lfRight,lfBottom,lfTop, 0.1f, 100.0f);
  m2DCamera.SetLookAt( cVec3(0.0f,0.0f,1.0f), cVec3(0.0f, 0.f, 0.f) );

  // Init the font
  cTextureManager::Get().Init( 256 ); // <- Cuanto Debería ser luiMaxSize para las texturas ??????????
//  mFont.Init( "./Src/Data/Fonts/Test1.fnt" );					// <<<<<<<<<<<<<<<<<<<< EL PROBLEMA ANDA POR AQUI: COMENTA  ESTO Y COMPILA! ! !

 

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

   mFont.Init( "./Src/Data/Fonts/Test1.fnt" );
   // Behaviour Setting
  cBehaviourManager::Get().Init() ;

	// Init LuaManager
  cLuaManager::Get().Init();
  RegisterLuaFunctions(); // C++ Functions Registered into Lua

  // REMEMBER THE FOLDERS STRUCTURE TO ACCESS THE FILES 
  cLuaManager::Get().DoFile( "./Src/Data/Scripts/CreatePatrol.lua"  );

  cSceneManager::Get().Init(1);
  mScene = cSceneManager::Get().LoadResource("TestLevel", "./Src/Data/Scene/dragonsmall.DAE");

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
//  mfTimeElapsed += lfTimeStep;
  
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
 	// 1) Clear buffers
	// ---------------------------------------------------------------------------------------
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
	// 2) Activate the 3D Camera
  	// ---------------------------------------------------------------------------------------
		cGraphicManager::Get().ActivateCamera( &m3DCamera );


  // 3) Render Solid 3D
  	// ---------------------------------------------------------------------------------------
	// Set the world matrix
		cMatrix lWorld;
		lWorld.LoadIdentity();
		cGraphicManager::Get().SetWorldMatrix(lWorld);
  
	//Render Debug Lines
		cGraphicManager::Get().DrawGrid();
	
		cGraphicManager::Get().DrawAxis(); 
    //Comented because there are too many lines
	//	cGraphicManager::Get().DrawPoint(cVec3( 1.5f, 0.0f, 1.5f),cVec3( 1.0, 0.0, 1.0));
	//	cGraphicManager::Get().DrawLine(cVec3( -1.5f, 0.0f, -1.5f), cVec3( -1.5f, 0.0f, 1.5f), cVec3( 1.0f, 1.0f, 0.0f));
	

	// Character Rendering  
 
		cCharacterManager::Get().Render();

	// 4) Render 3D with transparency
	// ---------------------------------------------------------------------------------------

	// 5) Activate 2D Camera
	// ---------------------------------------------------------------------------------------
		cGraphicManager::Get().ActivateCamera( &m2DCamera ); // comment this for IA

	// 6) Render 2D Elements
	// ---------------------------------------------------------------------------------------
		// Draw some strings
  	lWorld.LoadIdentity();
		cGraphicManager::Get().SetWorldMatrix(lWorld);
   

		mFont.SetColour( 1.0f, 0.0f, 0.0f);									 // <<<<<<<<<<<<<<<<<<<<  COMENTA  DESDE AQUI ...
		mFont.Write(0, 200,0, "Año Totó pingüino() !¡¿?", 0, FONT_ALIGN_CENTER);// Uncomment this for IA

		mFont.SetColour( 0.0f, 1.0f, 0.0f );
		mFont.WriteBox(150, 200, 0 ,100 , " Esto es un test \n MultiLinea", 0, FONT_ALIGN_CENTER );
																			// <<<<<<<<<<<<<<<<<<<<   HASTA AQUI Y COMPILA! ! !

	// 7) Postprocessing
	// ---------------------------------------------------------------------------------------

  //Render scene
  glDisable(GL_TEXTURE_2D);
 ((cScene *)mScene.GetResource())->Render();
  glEnable(GL_TEXTURE_2D);

	// 8) Swap Buffers
	// ---------------------------------------------------------------------------------------
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

  // Font Deinit
  mFont.Deinit();

  //Texture manager
  cTextureManager::Get().Deinit();

  //Scene deinit
  ((cScene *)mScene.GetResource())->Deinit();

  //Scene manager
  cSceneManager::Get().Deinit();

  //Deinit graphic manager
  bool lbResult = cGraphicManager::Get().Deinit();

  //Deinit window
	lbResult =  lbResult && cWindow::Get().Deinit();
  return lbResult;
}
 
void cGame::SetFinished(bool lbFinish)
{
  mbFinish = lbFinish;
}