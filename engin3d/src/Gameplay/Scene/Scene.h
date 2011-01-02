#ifndef SCENE_H
#define SCENE_H
#include "../../Utility/Resource.h"
#include "../../Utility/ResourceHandle.h"

struct aiScene;

class cScene : public cResource
{
  public:
    cScene(void) { mbLoaded = false; }
    virtual bool Init ( const std::string &lacNameId, const std::string &lacFile);
    virtual void Deinit();
    virtual bool IsLoaded() { return mbLoaded; }
    void Update(float lfTimeStep);
    void Render();

  private:
    void ProcessScene( const aiScene* lpScene );
    typedef std::vector<cResourceHandle> cResourceHandleList;
    typedef cResourceHandleList::iterator cResourceHandleListIt;
     
	cResourceHandleList mMeshList;
	cResourceHandleList mMaterialList;
	std::vector<unsigned> mMeshMaterialIndexList;
    std::string macFile;
    bool mbLoaded;
};
#endif
