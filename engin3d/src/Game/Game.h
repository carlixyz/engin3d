#ifndef CGAME_H
#define	CGAME_H
#include "../Utility/Singleton.h"

class cGame : public cSingleton<cGame>
{
 private:  
  friend class cSingleton<cGame>;
  bool mbFinish;
  float mfTimeElapsed;
 
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
 protected:
   cGame() {;}
};

#endif	/* CGAME_H */

