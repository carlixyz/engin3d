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
  mbFinish = false;
  mfTimeElapsed = 0.0f;

  cApplicationProperties mProperties;
  mProperties.macApplicationName = "Test 1";
  mProperties.mbFullscreen = false;
  mProperties.muiBits = 16;
  mProperties.muiWidth = 640 ;
  mProperties.muiHeight = 480;

  bool lbResult = cWindow::Get().Init( mProperties);
  return lbResult;
}

// Method to update the game
void cGame::Update(float lfTimeStep)
{
	cWindow::Get().Update();

  //Update time
  mfTimeElapsed += lfTimeStep;
  
  //If the time is over 5 seconds end the game
  if (mfTimeElapsed >= 5.0f) 
    mbFinish = true;

	mbFinish = mbFinish || cWindow::Get().GetCloseApplication();
	if (mbFinish)
	{ 
		return;
	}
}

//Method to render the game 
void cGame::Render()
{
  //Convert the time to string
  std::stringstream lssTimeStream;
  lssTimeStream << mfTimeElapsed;

  //Create the string
  std::string lsDebugString;
  lsDebugString = std::string("Time elapsed ") + lssTimeStream.str() + std::string("\n");

  //Print the string
#ifdef _WIN32
  OutputDebugString(lsDebugString.c_str());
#else
  std::cout << lsDebugString.c_str();
#endif

}

//Method to deinitialize the game
bool cGame::Deinit()
{
	bool lbResult = cWindow::Get().Deinit();
  return lbResult;
}
 