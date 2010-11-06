#include <windows.h>
#include <iostream>
#include <sstream>
#include "InputManager.h"
#include "../Window/Window.h"

//Initializate the input manager
void cInputManager::Init(const tActionMapping laActionMapping[], unsigned int luiActionCount)
{
  //Initializing the OIS Library
  OIS::ParamList pl;

  std::ostringstream wnd;
  wnd << (size_t)cWindow::Get().GetHWND();

  pl.insert(std::make_pair( std::string("WINDOW"), wnd.str()));

  //Default mode is foreground exclusive..but, we want to show mouse -
  //so nonexclusive
  pl.insert(std::make_pair(std::string("w32_mouse"),
  std::string("DISCL_FOREGROUND" )));
  pl.insert(std::make_pair(std::string("w32_mouse"),
  std::string("DISCL_NONEXCLUSIVE")));

  //This never returns null.. it will raise an exception on errors
  mpOISInputManager = OIS::InputManager::createInputSystem(pl);
  
  //Lets enable all addons that were compiled in: 
  mpOISInputManager->enableAddOnFactory(OIS::InputManager::AddOn_All);

  //Initalizate the devices
  cGenericDevice * lpKeyboard = new cKeyboard;
  lpKeyboard->Init();
  
  //Add the devices to the list of devices
  mDevices.push_back(lpKeyboard);

  //Initializate the actions vector
  mActions.resize(luiActionCount);

  //Initializate the mapping structure
  mMapped.resize(luiActionCount);
  int liIndex = 0;
  while ( laActionMapping[liIndex].miAction >= 0)
  {
    cInputEntry lEntry;
    lEntry.muiDevice = laActionMapping[liIndex].miDevice;
    lEntry.muiChannel = laActionMapping[liIndex].miChannel;
    mMapped[laActionMapping[liIndex].miAction].push_back(lEntry);

    ++liIndex;
  }
}


//Update the input manager
void cInputManager::Update(float lfTimeStep)
{

  //Update all devices
  int liSize = mDevices.size();
  for (int liDeviceId = 0; liDeviceId < liSize; ++liDeviceId){
    if (mDevices[liDeviceId] != NULL)
      mDevices[liDeviceId]->Update();
  }

  //For each action
  liSize = mMapped.size();
  for(int lActionId = 0; lActionId < liSize; ++lActionId)
  {
    //For each map
    int liMapSize = mMapped[lActionId].size();
    float lfValue = 0.0f;
    //Calculate the value of the action asking to all devices
    for (int liMapId = 0; liMapId < liMapSize; ++liMapId)
    {
      const cInputEntry &lEntry = mMapped[lActionId][liMapId];
      lfValue += Check(lEntry.muiDevice, lEntry.muiChannel);
    }

    // Establish the value of the action
    mActions[lActionId].Update(lfTimeStep,lfValue);
  }
}

//Deinitalizate the input Manager
void cInputManager::Deinit()
{
  //Deinit all devices and destroy the devices objects
  int liNumberDevices = mDevices.size();
  for (int liDeviceId = 0; liDeviceId < liNumberDevices; liDeviceId++)
  {
    mDevices[liDeviceId]->Deinit();
    delete mDevices[liDeviceId];
  }

  //Free memory from the action vector and map vector
  mActions.resize(0);
  int liMapSize = mMapped.size();
  for (int liMapId = 0; liMapId < liMapSize; liMapId++)
    mMapped[liMapId].resize(0);
  mMapped.resize(0);

  //Deinit OIS
  if( mpOISInputManager )
  {
    OIS::InputManager::destroyInputSystem(mpOISInputManager);
    mpOISInputManager = NULL;
  }
}

//Private method to check if a device is valid
float cInputManager::Check(int liDevice, int liChannel)
{
  if (mDevices[liDevice] != NULL && mDevices[liDevice]->IsValid())
    return mDevices[liDevice]->Check(liChannel);
  return 0.0f;
}

