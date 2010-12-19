#ifndef SCENE_H
#define SCENE_H
#include "../../Utility/Resource.h"

class cScene : public cResource
{
  public:
    cScene(void) { mbLoaded = false; }
    virtual bool Init ( const std::string &lacNameId,
      const std::string &lacFile);
    virtual void Deinit();
    virtual bool IsLoaded() { return mbLoaded; }
    void Update(float lfTimeStep);
    void Render();
  private:
    std::string macFile;
    bool mbLoaded;
};
#endif
