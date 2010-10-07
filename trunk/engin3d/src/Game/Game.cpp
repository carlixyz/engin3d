#include "Game.h"
#include "../Graphics/GraphicManager.h"
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
  glLoadIdentity();

  //Render here

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
 