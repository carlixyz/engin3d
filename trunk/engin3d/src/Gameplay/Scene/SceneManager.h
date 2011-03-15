#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "../../Utility/ResourceManager.h"
#include "../../Utility/Singleton.h"

#include "Scene.h"

class cSceneManager : public cResourceManager,
  public cSingleton<cSceneManager>
{
  public:
    friend class cSingleton<cSceneManager>;
  protected:
    cSceneManager() { ; } //Protected constructor
  private:
    virtual cResource * LoadResourceInternal( std::string lacNameID, 
      const std::string &lacFile);
};
#endif

