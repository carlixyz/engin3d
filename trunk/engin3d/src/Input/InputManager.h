#pragma once
#ifndef INPUT_MANAGER
#define INPUT_MANAGER

#include "../Utility/Singleton.h"
#include "Keyboard.h"
#include "InputEntry.h"
#include "InputAction.h"
#include <vector>

//Macro defined to have the code a little bit clean
#define IsPressed(ACTION) cInputManager::Get().GetAction(ACTION).GetIsPressed()
#define BecomePressed(ACTION) cInputManager::Get().GetAction(ACTION).GetBecomePressed()
#define IsReleased(ACTION) cInputManager::Get().GetAction(ACTION).GetIsReleased()

class OIS::InputManager;

//Device enumerator
enum eDevices
{
  eKeyboard = 0
};

//Struct to hold the mapping of the actions
struct tActionMapping
{
  int miAction;
  int miDevice;
  int miChannel;
};

//Class Input Manager
class cInputManager : public cSingleton<cInputManager>
{
  protected:
    cInputManager(){;}
  private:
    std::vector<cGenericDevice *> mDevices; //Hold the devices associated to the manager
    std::vector<cInputAction> mActions; ///Hold the actions
    std::vector< std::vector <cInputEntry> > mMapped; //Hold the map between actions and devices
    float Check(int liDevice, int liChannel);

    //IOS vard
    friend class cKeyboard;
    OIS::InputManager * mpOISInputManager;

  public:
    friend class cSingleton<cInputManager>;
    void Init(const tActionMapping laActionMapping[], unsigned luiActionCount); 
    void Update(float lfTimeStep);
    inline cInputAction &GetAction(const int &lActionId) { return mActions[lActionId]; }
    void Deinit(void);
};

#endif