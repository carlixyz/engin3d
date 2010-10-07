#include "../Utility/Singleton.h"
#include "GLHeaders.h"
#include "../Window/Window.h"

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

    friend class cSingleton<cGraphicManager>;
  protected:
    cGraphicManager() { ; }
 
};