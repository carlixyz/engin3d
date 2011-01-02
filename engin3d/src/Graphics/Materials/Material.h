#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include "MaterialData.h"
#include "../../Utility/Resource.h"
#include "../../Utility/ResourceHandle.h"

//struct aiMaterial;

class cMaterial : public cResource
{
  public:
    cMaterial() { mbLoaded = false; }

    virtual bool Init ( const std::string &lacNameID, void * lpMemoryData, int liTypeID);
    virtual void Deinit();
    virtual bool IsLoaded() { return mbLoaded; }
	void SetMaterial();
 
private:
    cResourceHandle mDiffuseTexture;

    bool mbLoaded;
};

#endif
