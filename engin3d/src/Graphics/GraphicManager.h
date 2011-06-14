#ifndef GRAPHIC_MANAGER_H
#define GRAPHIC_MANAGER_H

#include "../Utility/Singleton.h"
#include "GLHeaders.h"
#include "Camera.h"
#include "../Window/Window.h"
#include "../Libraries/MathLib/MathLib.h"

class cWindow;

class cGraphicManager : public cSingleton <cGraphicManager>
{
  private:
    cWindow* mpWindow;
    GLuint mPixelFormat;
    HGLRC mHRC;
	cCamera * mpActiveCamera;
	cMatrix mWorldMatrix;
	cMatrix mWVPMatrix;	//World view Projection for shader Effects

  public:
    bool Init( cWindow * lpWindow );
    bool Deinit();
    void SwapBuffer();
    bool CreateContext( cWindow *lpWindow );
    void InitializeGLState();

	void SetWorldMatrix( const cMatrix &lMatrix );
	void RefreshWorldView();
	void ActivateCamera( cCamera * lpCamera );

	//Auxiliar Tools for rendering
	void DrawPoint(const cVec3 &lvPosition, const cVec3 &lvColor ); 
	void DrawLine( const cVec3 &lvPosition1, const cVec3 &lvPosition2, const cVec3 &lvColor );
	void DrawGrid();
	void DrawAxis();

	cCamera * GetActiveCamera() { return mpActiveCamera; }
	const cMatrix &GetWVPMatrix() { return mWVPMatrix; }

    friend class cSingleton<cGraphicManager>;
  protected:
    cGraphicManager() { ; }
 
};

#endif