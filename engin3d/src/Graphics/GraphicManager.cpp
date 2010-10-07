#include "GraphicManager.h"
#include <assert.h>

//Method to init the class
bool cGraphicManager::Init( cWindow *lpWindow )
{
  bool lbRet = CreateContext( lpWindow );

  if (lbRet)
    InitializeGLState();

  return lbRet;
}

//Create context 
bool cGraphicManager::CreateContext( cWindow *lpWindow )
{
  //Ensure that window is not null
  assert(lpWindow);

  //Pixel description
  static PIXELFORMATDESCRIPTOR lPfp = 
  {
    sizeof(PIXELFORMATDESCRIPTOR),
    1,
    PFD_DRAW_TO_WINDOW |
    PFD_SUPPORT_OPENGL |
    PFD_DOUBLEBUFFER,
    PFD_TYPE_RGBA,
    lpWindow->GetBpp(),
    0, 0, 0, 0, 0, 0,
    0,
    0, 0, 0, 0, 0, 0,
    32, 0, 0,
    PFD_MAIN_PLANE,
    0, 0, 0, 0 
  };

  //Create the Device Context
  HDC &lDC = lpWindow->GetHDC();
  if (!lDC)
  {
    Deinit();
    MessageBox(NULL, "Can't Create A GL Device Context.",
      "ERROR", MB_OK|MB_ICONEXCLAMATION);
    return false;
  }

  //Create pixel format
  mPixelFormat = ChoosePixelFormat(lDC, &lPfp);
  if (!mPixelFormat)
  {
    Deinit();
    MessageBox(NULL, "Can't Find A Suitable PixelFormat", 
      "ERROR", MB_OK|MB_ICONEXCLAMATION);
    return false;
  }

  //Set pixel format  
  if (!SetPixelFormat(lDC, mPixelFormat, &lPfp))
  {
    Deinit();
    MessageBox(NULL, "Can't Set The PixelFormat.",
      "ERROR", MB_OK|MB_ICONEXCLAMATION);
    return false;
  }

  //Create context
  mHRC = wglCreateContext(lDC);
  if (!mHRC)
  {
    Deinit();
    MessageBox(NULL, "Can't Create A GL Rendering Context.", 
      "ERROR", MB_OK|MB_ICONEXCLAMATION);
    return false;
  }

  //Set the current context 
  if (!wglMakeCurrent(lDC, mHRC))
  {
    Deinit();
    MessageBox(NULL, "Can't Activate The GL Rendering Context.",
      "ERROR", MB_OK|MB_ICONEXCLAMATION);
    return false;
  }

  mpWindow = lpWindow;
  return true;
}

//Method to initialize OpenGL
void cGraphicManager::InitializeGLState()
{
  //Specify shade model
  glShadeModel(GL_SMOOTH);

  //Set the background color of the window
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  //Depth buffer initialization
  glClearDepth(1.0f);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  //Perspective and view initialization
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  unsigned muiWidth = mpWindow->GetWidth();
  unsigned muiHeight = mpWindow->GetHeight();
  glViewport(0, 0, muiWidth, muiHeight);

  //Enable use of texture
  glEnable(GL_TEXTURE_2D);

  //Set the trasnaparency behaviour
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  //Set the correct projection (perspective) 
  //with the aspect based on the size of the window
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  float lfAspectRatio = (float)muiWidth/(float)muiHeight;
  gluPerspective(45.0f, lfAspectRatio, 0.1f, 100.0f);

  //Put the identity matrix into model matrix
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

//Method to deinit the manager
bool cGraphicManager::Deinit()
{
  if (mHRC)
  {
    //Deinit GL context
    if (!wglMakeCurrent(NULL, NULL))
      MessageBox(NULL, "Release Of DC And RC Failed.",
      "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);

    //Destroy GL context
    if (!wglDeleteContext(mHRC))
      MessageBox(NULL, "Release Rendering Context Failed.",
      "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
    mHRC = NULL;
  }
  mpWindow = NULL;
  return true;     
}

//Method to swap buffers
void cGraphicManager::SwapBuffer()
{
  if ( mpWindow )
    SwapBuffers(mpWindow->GetHDC());
}