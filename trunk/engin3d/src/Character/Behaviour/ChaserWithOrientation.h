#ifndef CCHASERORIENTED_H
#define	CCHASERORIENTED_H

#include "BehaviourBase.h"
#include "../Character.h"

class cChaserWithOrientation : public cBehaviourBase
{
private:
	// pointer to Character 
	cCharacter * mpCharacter; 
	
	// Target Position
	cVec3 mTarget; 

  //Reached end point
  bool mbEndReached;

public:

	// initialize behaviours attribs 
	void Init(cCharacter * lpCharacter); 

	// Deinitialize the behaviour´s attribs 
	void Deinit(); 

	// special Implementation of the current behaviour 
	void Update(float lfTimestep); 

	void SetTarget(float posX, float posY, float posZ ); 

  //Check if the target has been reached 
  bool EndPointReached();

};

#endif /* CCHASERORIENTED */

