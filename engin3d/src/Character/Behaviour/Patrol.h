#ifndef CPATROL_H
#define	CPATROL_H

//#include "behaviourbase.h"
#include "ChaserWithOrientation.h"

class cPatrol : public cBehaviourBase
{
  cChaserWithOrientation* mpBehaviour;
  cCharacter * mpCharacter;
  cVec3 mTargetWaypoint;
  

  bool mbEndReached;
public:
  // initialize behaviours attribs 
  void Init(cCharacter * lpCharacter);
	
  // Deinitialize the behaviour´s attribs 
  void Deinit();
	
  // special Implementation of the current behaviour 
  void Update(float lfTimestep);
	
  void SetTarget(float posX, float posY, float posZ ); 
  
  inline const cVec3 &GetTargetWaypoint() const {return mTargetWaypoint;}
  
  //Check if the target has been reached 
  bool EndPointReached();
};

#endif

