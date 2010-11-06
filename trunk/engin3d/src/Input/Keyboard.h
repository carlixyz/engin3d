#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <Windows.h>
#include "GenericDevice.h"

#include "OISInputManager.h"
#include "OISException.h"
#include "OISKeyboard.h"
#include "OISEvents.h"

class cKeyboard : public cGenericDevice, public OIS::KeyListener
{  

  private:
    OIS::Keyboard* mpOISKeyboard; //OIS Device

    static const unsigned kuiInputChanelSize = 256; //Channel size
    bool mabInputBuffer[kuiInputChanelSize]; //Input buffer
    bool mbIsValid; // Device is valid

  public:
    cKeyboard() : mbIsValid(false) { ; }
    void Init();  
    void Deinit(void);
    void Update(void);
    float Check(unsigned luiEntry);
    inline bool IsValid(void) { return mbIsValid; }
  
    //   Listeners for the keyboard
    bool keyPressed( const OIS::KeyEvent &lArg );
    bool keyReleased( const OIS::KeyEvent &lArg );

};

#endif