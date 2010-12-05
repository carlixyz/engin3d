#include "Patrol.h"
#include "../../Lua/LuaManager.h"
#include "../CharacterManager.h"
#include "../../Game/Game.h"

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
   
   //Default radius
   mfAwareRadius = 3;

   //Default enemy
   miEnemyId = 5;

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
  cCharacter * lcEnemy = cCharacterManager::Get().SearchCharacter( miEnemyId );
  assert(lcEnemy);
  cVec3 lDistanceVec =  lcEnemy->GetPosition() - mpCharacter->GetPosition();

  //The enemy is not in aware radius
  if (lDistanceVec.Length() > mfAwareRadius)
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
  else
  {
    //Get enemy position and set it as target
    cVec3 lcEnemyPosition = lcEnemy->GetPosition();
    mpBehaviour->SetTarget(lcEnemyPosition.x, lcEnemyPosition.y, lcEnemyPosition.z);
    mpBehaviour->Update(lfTimestep);
    cVec3 lcDistanceVec =  lcEnemy->GetPosition() - mpCharacter->GetPosition();
    if (lcDistanceVec.Length() <= 0.2f)
      cGame::Get().SetFinished(true);
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

//Setter for aware radius
void cPatrol::SetAwareRadius(float lfAwareRadius)
{
  mfAwareRadius = lfAwareRadius;
}

//Setter for enemy id
void cPatrol::SetEnemyID(int liEnemyId)
{
  miEnemyId = liEnemyId;
}

