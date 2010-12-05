#include "Patrol.h"
#include "../../Lua/LuaManager.h"

//Initialize the class
void cPatrol::Init(cCharacter * lpCharacter)
 {
   assert(lpCharacter);

   //Set the character
   mpCharacter = lpCharacter;

   //Set the Waypoint
   mTargetWaypoint = lpCharacter->GetPosition();

   //Establish the behaviour
   mpBehaviour = (cChaserWithOrientation*) cBehaviourManager::Get().CreateBehaviour( eCHASER_WITH_ORIENTATION );
   mpBehaviour->Init(mpCharacter);
   
   //End reached
   mbEndReached = false;
 }

//Deinitializate class
void cPatrol::Deinit()
{
  if (mpBehaviour)
	{
		mpBehaviour->Deinit();
		cBehaviourManager::Get().DestroyBehaviour( mpBehaviour );
	}

  mpBehaviour = NULL;
  mpCharacter = NULL;
}

void cPatrol::Update(float lfTimestep)
{
  //Set the next control point
  mpBehaviour->SetTarget(mTargetWaypoint.x, mTargetWaypoint.y, mTargetWaypoint.z);
  //Update the Behaviour
  mpBehaviour->Update(lfTimestep);

  //If the control point is reached
  if (mpBehaviour->EndPointReached())
  {
    //Update to the next point
    bool lbOk = cLuaManager::Get().CallLua("NextEndPoint", mpCharacter->GetId(), (int *) NULL);
    assert(lbOk);
  }
}

//Set the new target
void cPatrol::SetTarget(float posX, float posY, float posZ )
{
  mTargetWaypoint = cVec3( posX, posY, posZ );
}


//Check if the end point is reached
bool cPatrol::EndPointReached()
{
  return mbEndReached;
}