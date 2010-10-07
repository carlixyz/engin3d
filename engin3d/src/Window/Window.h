#ifndef CWIN_H
#define	CWIN_H

#ifdef _WIN32
#include <windows.h>
#endif

#include "../Utility/Singleton.h"
#include "ApplicationProperties.h"

class cWindow : public cSingleton<cWindow> 
{
 private:  
  friend class cSingleton<cWindow>;
  HINSTANCE mInstance;
  HWND mWnd;
  HDC mDC;
  cApplicationProperties mProperties;
  bool mbCloseApplication;
  
  static LRESULT CALLBACK WndProc(	HWND lWnd,	UINT lMsg,	WPARAM lWParam,	LPARAM lLParam);

 public:
  //Method to initialize the Window
  bool Init(cApplicationProperties &lProperties);
  // Method to update the window
  void Update();
  //Method to deinitialize the window
  bool Deinit();

  //Method to get the width 
  inline unsigned GetWidth() { return mProperties.muiWidth; }

  //Method to get the height
  inline unsigned GetHeight() { return mProperties.muiHeight; }

  //Method to get the bpp
  inline unsigned GetBpp() { return mProperties.muiBits; }

  //Get DC
  inline HDC &GetHDC() { return mDC; }

  inline bool GetCloseApplication() { return mbCloseApplication; }
  inline HWND GetHWND() { return mWnd; }

 protected:
   cWindow() {;}
};

#endif