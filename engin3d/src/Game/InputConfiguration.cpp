#include "InputConfiguration.h"
#include "..\Input\InputManager.h"

//Action to devices mapping
tActionMapping kaActionMapping[] = {
  { eIA_CloseApplication, eKeyboard, OIS::KC_ESCAPE },
  { eIA_TurnLeft, eKeyboard, OIS::KC_LEFT },
  { eIA_TurnRight, eKeyboard, OIS::KC_RIGHT },
  { eIA_Advance, eKeyboard, OIS::KC_UP},
  { eIA_PlayJog, eKeyboard, OIS::KC_LSHIFT},
  { eIA_PlaySalute, eKeyboard, OIS::KC_LCONTROL},
  { -1, -1, -1}
};

