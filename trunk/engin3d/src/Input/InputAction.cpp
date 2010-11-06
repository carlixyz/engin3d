#include "InputAction.h"
#include <math.h>


//Initializate class input
void cInputAction::Init(void)
{
  mfValue = 0.0f;
  mfTimePressed = 0.0f;
  mbIsPressed = false;
  mbWasPressed = false;
  mbBecomeReleased = false;
  mbBecomePressed = false; 
}

//Update the state
void cInputAction::Update(float lfTimeStep, float lfValue)
{
  mbWasPressed = mbIsPressed;
  mbIsPressed = (lfValue > 0.0f);

  if (mbIsPressed && mbWasPressed)
    mfTimePressed += fabs(lfTimeStep);
  else
    mfTimePressed = 0.0f;

  mbBecomePressed = (mbIsPressed && !mbWasPressed);
  mbBecomeReleased = (mbWasPressed && !mbIsPressed);

  mfValue = lfValue;
}
    
