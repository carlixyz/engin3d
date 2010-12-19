#ifndef MESH_H
#define MESH_H

#include <string>
#include "../../Utility/Resource.h"


struct aiMesh;

class cMesh : public cResource
{
  public:
    cMesh() { mbLoaded = false; }
    virtual bool Init ( const std::string &lacNameID,
      void * lpMemoryData, int luiTypeID);
    virtual void Deinit();
    virtual bool IsLoaded() { return mbLoaded; }
    void RenderMesh();
  private:
    unsigned muiIndexCount;
    bool mbLoaded;
    unsigned mVboVertices;
    unsigned mVboNormals;
    unsigned mVboTexture;
    unsigned mVboIndex;
    bool mbHasNormals;
    bool mbHasTexture;
};

#endif
