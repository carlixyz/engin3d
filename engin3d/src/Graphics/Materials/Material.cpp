#include "Material.h"

#include "../GLHeaders.h"
#include <cassert>

/*
#include "../../Graphics/Meshes/assimp/Include/assimp.hpp"
#include "../../Graphics/Meshes/assimp/Include/aiScene.h"
#include "../../Graphics/Meshes/assimp/Include/aiPostProcess.h"

#include "../../Graphics/Meshes/assimp/Include/aiMaterial.h"
*/
#include "../../Graphics/Meshes/assimp/Include/aiTypes.h"

#include "MaterialData.h"
#include "../../Utility/FileUtils.h"
#include "../../Graphics/Textures/Texture.h"
#include "../../Graphics/Textures/TextureManager.h"

//Init material into memory
bool cMaterial::Init( const std::string &lacNameID, 
                 void * lpMemoryData, int luiTypeID)
{
	cMaterialData * lpMaterialData =  (cMaterialData*) lpMemoryData;
	aiMaterial * lpAiMaterial = lpMaterialData->mpMaterial;

	aiString lPath;
	unsigned luiUVIndex;
	lpAiMaterial->GetTexture(aiTextureType_DIFFUSE,0,&lPath,0,&luiUVIndex);

	std::string lacFilename = cFileUtils::GetFileName(lPath.data);
	std::string lDiffuserTextureFile = lpMaterialData->macPath+"/"+lacFilename;
	mDiffuseTexture = cTextureManager::Get().LoadResource( lDiffuserTextureFile, lDiffuserTextureFile);

  mbLoaded = true;
  return true;
}
void cMaterial::SetMaterial()
{
	//Set the diffuse texture
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	cTexture * lpTexture = (cTexture*)mDiffuseTexture.GetResource();
	unsigned luiTextureHandle = lpTexture->GetTextureHandle();
	glBindTexture( GL_TEXTURE_2D, luiTextureHandle );
}


//Deinit materials
void cMaterial::Deinit()
{
;
}

