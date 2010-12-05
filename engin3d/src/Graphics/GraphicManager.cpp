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

void cGraphicManager::SetWorldMatrix( const cMatrix &lMatrix )
{
	mWorldMatrix = lMatrix;
	RefreshWorldView();
}

void cGraphicManager::RefreshWorldView()
{
	//Select the ModelView Matrix
	glMatrixMode(GL_MODELVIEW);
	// Calculate the modelView Matrix
	cMatrix lWorldView = mpActiveCamera->GetView();
	lWorldView = mWorldMatrix * lWorldView;
	// Set the View Matrix
	glLoadMatrixf( lWorldView.AsFloatPointer() );
}

void cGraphicManager::ActivateCamera(cCamera *lpCamera)
{
	assert(lpCamera);
	mpActiveCamera = lpCamera;

	// Select The Projection Matrix
	glMatrixMode(GL_PROJECTION);

  glLoadIdentity();
	// Set The Projection Matrix
	glLoadMatrixf( mpActiveCamera->GetProj().AsFloatPointer() );
	//Refresh the world View matrix
	RefreshWorldView();

}

// Render Auxiliar Tools
void cGraphicManager::DrawPoint(const cVec3 &lvPosition, const cVec3 &lvColor)
{
	glDisable(GL_TEXTURE_2D);

	//POINT
	glPointSize(10);
	glColor3f(lvColor.x, lvColor.y, lvColor.z);
	glBegin(GL_POINTS);
	glVertex3f(lvPosition.x, lvPosition.y, lvPosition.z);
	glEnd();

	glEnable(GL_TEXTURE_2D);
}

void cGraphicManager::DrawLine(const cVec3 &lvPosition1, const cVec3 &lvPosition2, const cVec3 &lvColor)
{
	glDisable(GL_TEXTURE_2D);

	//LINE
	glLineWidth(2);
	glColor3f(lvColor.x, lvColor.y, lvColor.z);
	glBegin(GL_LINES);

	glVertex3f(lvPosition1.x, lvPosition1.y, lvPosition1.z);
	glVertex3f(lvPosition2.x, lvPosition2.y, lvPosition2.z);

	glEnd();
	glEnable(GL_TEXTURE_2D);
}

void cGraphicManager::DrawGrid()
{
	glDisable(GL_TEXTURE_2D);

	//GRID
	glLineWidth(1);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINES);
	for (float lfxtmp = -10.0; lfxtmp <= 10.0; lfxtmp += 1.0)
	{
		glVertex3f( lfxtmp, 0.0f, -10.0f);
		glVertex3f( lfxtmp, 0.0f,  10.0f);
		glVertex3f( -10.0f, 0.0f, lfxtmp);
		glVertex3f(  10.0f, 0.0f, lfxtmp);
	};
	glEnd();

	glEnable(GL_TEXTURE_2D);
}

void cGraphicManager::DrawAxis()
{
	glDisable(GL_TEXTURE_2D);

	// AXIS
	glLineWidth(2);
	glBegin(GL_LINES);

	glColor3f( 1.0f, 0.0f, 0.0f);	// X Axis
	glVertex3f(0.0f, 0.0f, 0.0f);
	glColor3f( 1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);

	glColor3f( 0.0f, 1.0f, 0.0f);	// Y Axis
	glVertex3f(0.0f, 0.0f, 0.0f);
	glColor3f( 0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);

	glColor3f( 0.0f, 0.0f, 1.0f);	// Z Axis
	glVertex3f(0.0f, 0.0f, 0.0f);
	glColor3f( 0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);

	glEnd();

	glEnable(GL_TEXTURE_2D);
}
