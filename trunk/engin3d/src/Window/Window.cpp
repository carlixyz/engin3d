#include "Window.h"

LRESULT CALLBACK cWindow::WndProc(	HWND	lWnd,
									UINT	lMsg,
									WPARAM	lWParam,
									LPARAM	lLParam)
	{
		switch (lMsg)
		{
			case WM_CLOSE:
			{
				PostQuitMessage(0);
				return 0;
			}
			case WM_SYSCOMMAND:
			{
				switch (lWParam)
				{
					case SC_SCREENSAVE:
					case SC_MONITORPOWER:
						return 0; // Interrupt the action
				}
				break;
			}
		}	
		// Pass All Unhandled Messages To DefWindowProc
		return DefWindowProc(lWnd,lMsg,lWParam,lLParam);
	}

  bool cWindow::Init(cApplicationProperties &lProperties)
  {
	  mbCloseApplication = false;

	// 1- Init vars
	  mDC = NULL;
	  mWnd = NULL;
	  mInstance = GetModuleHandle(NULL);
	  mProperties = lProperties;

	// 2- Register the window
	  // Windows Class Structure
	  WNDCLASS lWndClass;
	  // Redraw On Size, and Own DC For Window.
	  lWndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	  // WndProc Handles Messages
	  lWndClass.lpfnWndProc = (WNDPROC) WndProc;
	  // No Extra Window Data
	  lWndClass.cbClsExtra = 0;
	  // No Extra Window Data
	  lWndClass.cbWndExtra = 0;
	  // Set the instance
	  lWndClass.hInstance = mInstance;
	  // Load the default Icon
	  lWndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	  // Load the Arrow Pointer
	  lWndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	  // No Background Required for GL
	  lWndClass.hbrBackground = NULL;
	  // We Don´t Want a Menu
	  lWndClass.lpszMenuName = NULL;
	  // Set the Class Name
	  lWndClass.lpszClassName = lProperties.macApplicationName.c_str();
	  

	  int liRet = RegisterClass(&lWndClass);
	  if (!liRet) // Attempt To Register The Window Class
	  {
		  DWORD lError = GetLastError();
		  MessageBox(NULL,"Failed to Register the Window","ERROR",MB_OK|MB_ICONEXCLAMATION);
		  return false;
	  }

	// 3- Preparation of Window for FullScreen 
	  if (lProperties.mbFullscreen)
	  {
		  // Device Mode
		  DEVMODE lScreenSettings;
		  // Clear the memory
		  memset(&lScreenSettings, 0,sizeof(lScreenSettings));
		  lScreenSettings.dmSize = sizeof(lScreenSettings);
		  lScreenSettings.dmPelsWidth = lProperties.muiWidth;
		  lScreenSettings.dmPelsHeight = lProperties.muiHeight;
		  lScreenSettings.dmBitsPerPel = lProperties.muiBits;
		  lScreenSettings.dmFields = DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		  // Try to Set Selected Mode and Get Results.
		  LONG lResult = ChangeDisplaySettings(&lScreenSettings, CDS_FULLSCREEN);
		  if (lResult != DISP_CHANGE_SUCCESSFUL)
		  {
			  if (MessageBox(NULL,"The Requested FullScreen Mode Is Not Supported By\n Your Video Card. Use Windowed Mode Instead?",
				  "ERROR", MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			  {
				  mProperties.mbFullscreen = false;
			  }
			  else
			  {
				  MessageBox(NULL,"Program Will Now Close.","ERROR", MB_OK|MB_ICONSTOP);
				  return false;
			  }
		  }
	  }

	// 4- Window Creation
	  DWORD lExStyle;
	  DWORD lStyle;
	  if (mProperties.mbFullscreen)
	  {
		  // Window Extended Style
		  lExStyle=WS_EX_APPWINDOW;
		  // Windows Style
		  lStyle=WS_POPUP;
		  // Hide Mouse Pointer
		  ShowCursor(FALSE);
	  }
	  else
	  {
		  //Window Extended Style
		  lExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		  // Windows Style
		  lStyle = WS_OVERLAPPEDWINDOW;
	  }

	  RECT lWindowRect;// Setting Dimensions
	  lWindowRect.left = (long)0;
	  lWindowRect.right = (long) lProperties.muiWidth;
	  lWindowRect.top = (long)0;
	  lWindowRect.bottom = (long) lProperties.muiHeight;

	  AdjustWindowRectEx(&lWindowRect, lStyle, FALSE, lExStyle);

	  // Create The Window
	  mWnd = CreateWindowEx(lExStyle,
		  lProperties.macApplicationName.c_str(),
		  lProperties.macApplicationName.c_str(),
		  lStyle |
		  WS_CLIPSIBLINGS |
		  WS_CLIPCHILDREN,
		  0, 0,
		  lWindowRect.right-lWindowRect.left,
		  lWindowRect.bottom-lWindowRect.top,
		  NULL,
		  NULL,
		  mInstance,
		  NULL);

	  if (!mWnd)
	  {
		  Deinit();	// Reset the Display
		  MessageBox(NULL,"Window Creation Error.",
			  "ERROR", MB_OK|MB_ICONEXCLAMATION);
		  return false;
	  }

	  ShowWindow(mWnd,SW_SHOW);
	  SetForegroundWindow(mWnd);
	  SetFocus(mWnd);	  

  // 5- Access to Device´s context
	  mDC = GetDC(mWnd);
	  if (!mDC)
	  {
		  Deinit();
		  MessageBox(NULL,"Can´t access the HDC.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		  return false;
	  }  

	  return true;
  }

  void cWindow::Update()
  {
	  MSG lMsg;
	  // Is There A Message Waiting?
	  if (PeekMessage(&lMsg,NULL,0,0,PM_REMOVE))
	  {
		  // Have we Receibed a Quit Message ?
		  if (lMsg.message == WM_QUIT)
		  {
			  mbCloseApplication = true;
		  }
		  else
		  {
			  TranslateMessage(&lMsg);
			  DispatchMessage(&lMsg);
		  }
	  }
  }

  bool cWindow::Deinit()
  {
	  if (mProperties.mbFullscreen)
	  {
		  // If So switch Back To The Desktop Resolution
		  ChangeDisplaySettings(NULL, 0);
		  ShowCursor(TRUE);
	  }

	  if (mDC && !ReleaseDC(mWnd, mDC))
	  {
		  mDC = NULL;
		  return false;
	  }

	  if (mWnd && !DestroyWindow(mWnd))
	  {
		  mWnd = NULL;
		  return false;
	  }

	  if (!UnregisterClass(mProperties.macApplicationName.c_str(), mInstance))
	  {
		  mInstance = NULL;
		  return false;
	  }
	  return true;
  }