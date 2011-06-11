#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>
#include "../../Utility/Resource.h"
#include "../../Utility/ResourceHandle.h"

static int kuiStaticMesh = 0;
static int kuiSkeletalMesh = 1;

struct aiMesh;

class cMesh : public cResource
{
  public:
    cMesh() { mbLoaded = false; }

    virtual bool Init ( const std::string &lacNameID, void * lpMemoryData, int luiTypeID);
	void Reload( void * lpMemoryData );
    virtual void Deinit();
    virtual bool IsLoaded()					{ return mbLoaded; }
    virtual void RenderMesh();
	virtual void Update(float lfTimestep)	{ }

	virtual void PrepareRender(cResourceHandle lMaterial) 	{;}


  protected:
    bool mbLoaded; 
	std::string macFile;
	unsigned muiIndexCount; 
    unsigned mVboVertices;
    unsigned mVboNormals; 
	std::vector<unsigned> maVboTexture; 
    unsigned mVboIndex; 

	bool mbHasNormals; // 
    bool mbHasTexture;  //
};

#endif
