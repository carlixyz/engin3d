#include "Keyboard.h"
#include <string.h>
#include "InputManager.h"
#include <assert.h>

//Initializate the keyboard input
void cKeyboard::Init()
{
  // Clear the input buffer
  memset(mabInputBuffer, 0, kuiInputChanelSize);

  //Build OIS keyboard object
  assert(cInputManager::Get().mpOISInputManager);
  OIS::InputManager * lpOISInputManager =cInputManager::Get().mpOISInputManager;
  mpOISKeyboard = (OIS::Keyboard*)lpOISInputManager->createInputObject(OIS::OISKeyboard, true );

  //Establish the callback
  mpOISKeyboard->setEventCallback( this );
  //Sucessfull initialization
  mbIsValid = true;
}


//Deinitializate the keyboard input and restore the original controller
void cKeyboard::Deinit()
{
  //Deinit OIS keyboard object
  assert(cInputManager::Get().mpOISInputManager);
  cInputManager::Get().mpOISInputManager->destroyInputObject(mpOISKeyboard);
  mpOISKeyboard = 0;

  //Establish the state of the class to uninitialized
  mbIsValid = false;
}

//Callback method when a key is pressed
bool cKeyboard::keyPressed( const OIS::KeyEvent &lArg )
{
  mabInputBuffer[lArg.key] = true;
  return true;
}

//Callback method when a key is releassed
bool cKeyboard::keyReleased( const OIS::KeyEvent &lArg )
{
  mabInputBuffer[lArg.key] = false;
  return true;
}

//Check is a key is pressed
float cKeyboard::Check(unsigned luiEntry)
{
  if (mabInputBuffer[luiEntry])
   return 1.0f;
  return 0.0f;
}

//Update keyboard
void cKeyboard::Update(void)
{
  //This fires off buffered events for keyboards
  assert(mpOISKeyboard);
  mpOISKeyboard->capture();
}

