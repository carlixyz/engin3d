#include "SceneManager.h"
#include "Scene.h"

cResource * cSceneManager::LoadResourceInternal(std::string lacNameID, const std::string &lacFile)
{
  //Create scene
  cScene * lpScene = new cScene();

  //Load scene 
  if (!lpScene->Init( lacNameID, lacFile ) )
  {
    delete lpScene;
    return NULL;
  }

  //Return scene as resource
  return lpScene;
}