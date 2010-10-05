#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "Game/Game.h"
#include "Utility/Util.h"

#ifdef _WIN32
int WINAPI WinMain( HINSTANCE hinstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main(int argc, char **argv)
#endif
{
  cGame::Get().Init();

  unsigned long luiLastTime = GetMiliSeconds();
  while (!cGame::Get().HasFinished())
  {
    //Calculate the time elapsed in seconds
    unsigned long luiActualTime = GetMiliSeconds();
    float lfTimeStep = ((float)(luiActualTime - luiLastTime) / 1000.0f);
    luiLastTime = luiActualTime;

    //Update
    cGame::Get().Update(lfTimeStep);

    //Render
    cGame::Get().Render();
  }
  //Deinit
  cGame::Get().Deinit();
  return 0;
}