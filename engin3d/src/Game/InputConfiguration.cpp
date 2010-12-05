#include "InputConfiguration.h"
#include "..\Input\InputManager.h"

//Action to devices mapping
tActionMapping kaActionMapping[] = {
  { eIA_CloseApplication, eKeyboard, OIS::KC_ESCAPE },
  { eIA_TurnLeft, eKeyboard, OIS::KC_LEFT },
  { eIA_TurnRight, eKeyboard, OIS::KC_RIGHT },
  { eIA_Advance, eKeyboard, OIS::KC_UP},
  { -1, -1, -1}
};

