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

public:

	// initialize behaviours attribs 
	void Init(cCharacter * lpCharacter); 

	// Deinitialize the behaviour´s attribs 
	void Deinit(); 

	// special Implementation of the current behaviour 
	void Update(float lfTimestep); 

	void SetTarget(float posX, float posY, float posZ ); 

};

#endif /* CCHASERORIENTED */

