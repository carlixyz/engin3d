#include "Game.h"
#include <string>
#include <sstream>
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

    // Character Creation 1 ( With Orientation )
  cCharacterManager::Get().Init();
  cCharacter* lpCharacter1 = cCharacterManager::Get().CreateCharacter();
  lpCharacter1->SetPosition( cVec3( 2.0f, 0.0f, 5.0f) );
  lpCharacter1->SetSpeed( 3.0f );
  lpCharacter1->SetYaw( 1.0f );
  lpCharacter1->SetAngSpeed( 2.0f );
  
    // Character Creation 2 ( Without Orientation )
  cCharacterManager::Get().Init();
  cCharacter* lpCharacter2 = cCharacterManager::Get().CreateCharacter();
  lpCharacter2->SetPosition( cVec3( -2.0f, 0.0f, -5.0f) );
  lpCharacter2->SetSpeed( 1.0f );
  lpCharacter2->SetYaw( -5.0f );

      // Character Creation 3 ( Snap orientation )
  cCharacterManager::Get().Init();
  cCharacter* lpCharacter3 = cCharacterManager::Get().CreateCharacter();
  lpCharacter3->SetSpeed( 2.0f );
  
  // Behaviour Setting
  cBehaviourManager::Get().Init() ;
  cBehaviourBase* lpBehaviour1 = cBehaviourManager::Get().CreateBehaviour( eCHASER_WITH_ORIENTATION );
  cBehaviourBase* lpBehaviour2 = cBehaviourManager::Get().CreateBehaviour( eCHASER_SNAP_ORIENTATION );
  cBehaviourBase* lpBehaviour3 = cBehaviourManager::Get().CreateBehaviour( eCHASER_NO_ORIENTATION );

  lpCharacter1->SetActiveBehaviour( lpBehaviour1 );
  lpCharacter2->SetActiveBehaviour( lpBehaviour2 );
  lpCharacter3->SetActiveBehaviour( lpBehaviour3 );

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

  //Deinit graphic manager
  bool lbResult = cGraphicManager::Get().Deinit();

  //Deinit window
	lbResult =  lbResult && cWindow::Get().Deinit();
  return lbResult;
}
 