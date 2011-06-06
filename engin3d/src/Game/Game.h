#ifndef CGAME_H
#define	CGAME_H
#include "../Utility/Singleton.h"
#include "../Graphics/GraphicManager.h"
#include "../Graphics/Fonts/FontManager.h"

class cGame : public cSingleton<cGame>
{
 private:  
  friend class cSingleton<cGame>;
  bool mbFinish;
//float mfAcTime; // not needed, same as Time elapsed
  float mfTimeElapsed;

// cFont mFont; // Added with font manager
  cCamera m3DCamera;
  cCamera m2DCamera;
  cResourceHandle mScene;
  cResourceHandle mFontHandle;

public:
  //Method to initialize the game
  bool Init();
  // Method to update the game
  void Update(float lfTimeStep);
  //Method to redner the game 
  void Render();
  //Method to deinitialize the game
  bool Deinit();
  //Method to get the state of the game
  inline bool HasFinished() { return mbFinish; }
  //Set the game finished 
  void SetFinished(bool lbFinish);

  inline float GetAcumulatedTime() { return mfTimeElapsed; }
 protected:
   cGame() {;}
};

#endif	/* CGAME_H */

