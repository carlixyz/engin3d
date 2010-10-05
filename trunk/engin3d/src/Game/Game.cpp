#include "Game.h"
#include <string>
#include <sstream>
#ifdef _WIN32
  #include <windows.h>
#endif

//Method to initialize the game
bool cGame::Init()
{
  mbFinish = false;
  mfTimeElapsed = 0.0f;
  return true;
}

// Method to update the game
void cGame::Update(float lfTimeStep)
{
  //Update time
  mfTimeElapsed += lfTimeStep;
  
  //If the time is over 5 seconds end the game
  if (mfTimeElapsed >= 5.0f) 
    mbFinish = true;
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
  return true;
}
 