#include "../Utility/Singleton.h"
#include "GLHeaders.h"
#include "../Window/Window.h"
#include "../MathLib/MathLib.h"

class cWindow;
class cGraphicManager : public cSingleton <cGraphicManager>
{
  private:
    cWindow* mpWindow;
    GLuint mPixelFormat;
    HGLRC mHRC;
  public:
    bool Init( cWindow * lpWindow );
    bool Deinit();
    void SwapBuffer();
    bool CreateContext( cWindow *lpWindow );
    void InitializeGLState();

	//Auxiliar Tools for rendering
	void DrawPoint(const cVec3 &lvPosition, const cVec3 &lvColor ); 
	void DrawLine( const cVec3 &lvPosition1, const cVec3 &lvPosition2, const cVec3 &lvColor );
	void DrawGrid();
	void DrawAxis();

    friend class cSingleton<cGraphicManager>;
  protected:
    cGraphicManager() { ; }
 
};