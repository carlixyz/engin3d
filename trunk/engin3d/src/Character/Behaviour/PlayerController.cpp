#include "PlayerController.h"
#include "../../Game/InputConfiguration.h"
#include "../../Input/InputManager.h"

//Initialize the class
void cPlayerController::Init(cCharacter *lpCharacter)
{
  mpCharacter = lpCharacter;
}

//Deinitialize the class
void cPlayerController::Deinit()
{
}

//Update the position of the player based on the key pressed
void cPlayerController::Update(float lfTimestep)
{
  //Move forward
  if (IsPressed(eIA_Advance))
  {
    float lfDisplacement = mpCharacter->GetSpeed() * lfTimestep;
    cVec3 lVecPos = mpCharacter->GetPosition() + ( lfDisplacement * mpCharacter->GetFront() );
	  mpCharacter->SetPosition(lVecPos);
  }

  //Turn right
  if (IsPressed(eIA_TurnRight))
  {
    float lfAngle = mpCharacter->GetAngSpeed() * lfTimestep;
		mpCharacter->SetYaw( mpCharacter->GetYaw() - lfAngle );
  }

  //Turn left
  if (IsPressed(eIA_TurnLeft))
  {
    float lfAngle = mpCharacter->GetAngSpeed() * lfTimestep;
		mpCharacter->SetYaw( mpCharacter->GetYaw() + lfAngle );
  }

}

//Check if the player has reach the end point
//Always yes because is user controlled
bool cPlayerController::EndPointReached()
{
  return true;
}