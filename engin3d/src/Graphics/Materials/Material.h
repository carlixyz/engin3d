#ifndef MATERIAL_H
#define MATERIAL_H

#include <vector>
#include <sstream>
#include "MaterialData.h"

#include "../../Utility/Resource.h"
#include "../../Utility/ResourceHandle.h"

struct cTextureData
{
	std::string macShaderTextureID;	// Shader Textures 
	cResourceHandle mTexture;
};
//---------------------------------------------------------------------------------------
class cMaterial : public cResource
{
  public:
    cMaterial() { mbLoaded = false; }

    virtual bool Init( const std::string &lacNameID, void * lpMemoryData, int liTypeID);
	virtual bool Init( const std::string &lacNameID, const std::string &lacFile );

    virtual void Deinit();
    virtual bool IsLoaded() { return mbLoaded; }
	void PrepareRender();
	bool SetFirstPass();
	bool SetNextPass();

	cResourceHandle GetEffect() { return mEffect; }
 
private:
	void ReadAllTextures(aiMaterial * lpAiMaterial, cMaterialData * lpMaterialData);
	std::string macFile;
	std::vector<cTextureData> maTextureData;
    cResourceHandle mEffect;
    bool mbLoaded;
};

#endif
