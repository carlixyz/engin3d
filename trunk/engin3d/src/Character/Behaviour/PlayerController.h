#ifndef CPLAYERCONTROLLER_H
#define	CPLAYERCONTROLLER_H

#include "behaviourbase.h"
#include "../Character.h"

class cPlayerController : public cBehaviourBase
{
private:
	// pointer to Character 
	cCharacter * mpCharacter; 
	
public:

	// initialize behaviours attribs 
	void Init(cCharacter * lpCharacter); 

	// Deinitialize the behaviour´s attribs 
	void Deinit(); 

	// special Implementation of the current behaviour 
	void Update(float lfTimestep); 

  //Dummy method 
  void SetTarget(float posX, float posY, float posZ){}
  //Check if the target has been reached 
  bool EndPointReached();

};

#endif