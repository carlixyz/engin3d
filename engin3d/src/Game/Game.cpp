#include "Game.h"
#include <string>
#include <sstream>

#ifdef _WIN32
  #include <windows.h>
  #include "../Window/Window.h" 
#endif

//Method to initialize the game
bool cGame::Init()
{
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
  m3DCamera.SetLookAt( cVec3(5.0f, 5.0f, 5.0f), cVec3(0.0f, 0.0f, 0.0f), cVec3( 0.0, 1.0f, 0.0f) );


  bool lbResult = cWindow::Get().Init( mProperties);
  
  //Init OpenGL
  if (lbResult)
  {
    lbResult = cGraphicManager::Get().Init( &cWindow::Get() );

    //If something failed kill the window
    if (!lbResult)
      cWindow::Get().Deinit();
  }
  return lbResult;
}

// Method to update the game
void cGame::Update(float lfTimeStep)
{
	cWindow::Get().Update();

  //Update time
  mfTimeElapsed += lfTimeStep;
  
	mbFinish = mbFinish || cWindow::Get().GetCloseApplication();
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
  
  //Activatethe 3D Camera
  cGraphicManager::Get().ActivateCamera( &m3DCamera );

  // Set the world matrix
  cMatrix lWorld;
  lWorld.LoadIdentity();
  cGraphicManager::Get().SetWorldMatrix(lWorld);
  
  //Render Debug Lines
  cGraphicManager::Get().DrawGrid();
  cGraphicManager::Get().DrawAxis();
  cGraphicManager::Get().DrawPoint(cVec3( 1.5f, 0.0f, 1.5f),cVec3( 1.0, 0.0, 1.0));
  cGraphicManager::Get().DrawLine(cVec3( -1.5f, 0.0f, -1.5f), cVec3( -1.5f, 0.0f, 1.5f), cVec3( 1.0f, 1.0f, 0.0f));
   
//  cGraphicManager::Get().SwapBuffer();

  lWorld.LoadTranslation(cVec3(1, 0, -1.5) );
  cMatrix lRot;
  lRot.LoadRotation(cVec3(0.0f, 1.0f, 0.0f), 3.14f / 2.0f );
  cGraphicManager::Get().SetWorldMatrix(lWorld * lRot );
  cGraphicManager::Get().DrawAxis();
  cGraphicManager::Get().SwapBuffer();

}

//Method to deinitialize the game
bool cGame::Deinit()
{
  //Deinit graphic manager
  bool lbResult = cGraphicManager::Get().Deinit();

  //Deinit window
	lbResult =  lbResult && cWindow::Get().Deinit();
  return lbResult;
}
 