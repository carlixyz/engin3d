#include "Material.h"

#include "../GLHeaders.h"
#include <cassert>

#include "../../Utility/FileUtils.h"
#include "../Textures/TextureManager.h"

#include "../Effects/EffectManager.h"
#include "../Effects/Effect.h"
#include "../GraphicManager.h"
#include "../Camera.h"
#include "../../Game/Game.h"

struct cAssimpTextureMaping
{
	aiTextureType mTextureType;
	std::string macPrefix;
};

static cAssimpTextureMaping kacTextureMapping[] =
{
	{ aiTextureType_DIFFUSE, "Diffuse_" }, 
	// The texture is combined with the result of the diffuse lighting equation.
	{ aiTextureType_SPECULAR, "Specular_" }, 
	// The texture is combined with the result of the specular lighting equation.
	{ aiTextureType_AMBIENT, "Ambient_" }, 
	// The texture is combined with the result of the ambient lighting equation.
	{ aiTextureType_EMISSIVE, "Emissive_" }, 
	// The texture is added to the result of the lighting calculation. It isn't influenced by incoming light.
	{ aiTextureType_HEIGHT, "Height_" }, 
	// The texture is a height map. By convention, higher gray-scale values stand for higher elevations from the base height.
	{ aiTextureType_NORMALS, "Normals_" }, 
	// The texture is a (tangent space) normal-map. Again, there are several conventions for tangent-space normal maps.
	// Assimp does (intentionally) not distinguish here.
	{ aiTextureType_SHININESS, "Shininess_" }, // The texture defines the glossiness of the material. 
	// The glossiness is in fact the exponent of the specular (phong) lighting equation. 
	// Usually there is a conversion function defined to map the linear color values in the texture to a suitable exponent.
	{ aiTextureType_OPACITY, "Opacity_" }, // The texture defines per-pixel opacity.  
	// Usually 'white' means opaque and 'black' means 'transparency'. Or quite the opposite.
	{ aiTextureType_DISPLACEMENT, "Displacement_"},  // Displacement texture. The exact purpose and format is application-dependent. 
	// Higher color values stand for higher vertex displacements.
	{ aiTextureType_LIGHTMAP, "Lightmap_" }, // Lightmap texture (aka Ambient Occlusion). Both 'Lightmaps' and dedicated 
	// 'ambient occlusion maps' are covered by this material property.  
	// The texture contains a scaling value for the final color value of a pixel. Its intensity is not affected by incoming light.
	{ aiTextureType_REFLECTION, "Reflexion_" },   // Reflection texture. Contains the color of a perfect mirror reflection.
	//  Rarely used, almost never for real-time applications.
	{ aiTextureType_UNKNOWN, "Unknown_" },   // Unknown texture. A texture reference that does not match any of the 
	// definitions above is considered to be 'unknown'. It is still imported, but is excluded from any further postprocessing.
	{ aiTextureType_NONE, "NONE" }      // Mark for the end of the stucture
};

//Init material into memory
bool cMaterial::Init( const std::string &lacNameID, const std::string &lacFile )
{
   // Load an XML with a material
   macFile = lacFile;
   // Load the XML file
   TiXmlDocument doc( lacFile.c_str() );

   bool lbLoadOk = doc.LoadFile();

   if ( lbLoadOk )
   {
	   OutputDebugString("\n Material->Init XML Load: OK\n");
   }
   else
   {
      OutputDebugString("\n Material->Init XML Load: FAILED\n");
	  return false;
   }

   TiXmlHandle lhDoc(&doc);
   TiXmlElement* lpElem;
   TiXmlHandle lhRoot(0);
   lpElem = lhDoc.FirstChildElement().Element();
   assert(lpElem);

   // save this for later
   lhRoot = TiXmlHandle(lpElem);
   std::string lacEffectName = lhRoot.ToElement()->Attribute("effectName");
   std::string lacEffectPath = lhRoot.ToElement()->Attribute("effectPath");

   mEffect = cEffectManager::Get().LoadResource( lacEffectName, lacEffectPath );

   // Read all the animations
   maTextureData.resize(0);
   lpElem=lhRoot.FirstChild( "Texture" ).Element();

   for( lpElem; lpElem; lpElem = lpElem->NextSiblingElement("Texture"))
   {
		cTextureData lData;
		std::string lacTextureFile = lpElem->Attribute("file");
		lData.macShaderTextureID = lpElem->Attribute("name");
		lData.mTexture = cTextureManager::Get().LoadResource(lacTextureFile, lacTextureFile);
	
		assert(lData.mTexture.IsValidHandle());
		maTextureData.push_back(lData);
   }

   mbLoaded = mEffect.IsValidHandle();
   return mbLoaded;
}


bool cMaterial::Init( const std::string &lacNameID, void * lpMemoryData, int luiTypeID)
{
	macFile = "";
	cMaterialData * lpMaterialData =  (cMaterialData*) lpMemoryData;
	aiMaterial * lpAiMaterial = lpMaterialData->mpMaterial;

	aiString name;
	lpAiMaterial->Get( AI_MATKEY_NAME, name);
	ReadAllTextures(lpAiMaterial, lpMaterialData );

	//Load the Shader using the material name
	char lacEffectName[128];
	strcpy(lacEffectName, name.data);
	char * lacLimit = strstr(lacEffectName, "_");
	if (lacLimit != 0)
	{
		*lacLimit = '\0';
	}

	std::stringstream lShaderPath;
	lShaderPath << "./Src/Data/Shader/" << lacEffectName << ".fx" ;
	std::string lsCheck = lShaderPath.str();

	mEffect = cEffectManager::Get().LoadResource( lacEffectName, lShaderPath.str() );

	assert(mEffect.IsValidHandle());
	mbLoaded = mEffect.IsValidHandle();
	return mEffect.IsValidHandle();
/*
	aiString lPath;
	unsigned luiUVIndex;
	lpAiMaterial->GetTexture(aiTextureType_DIFFUSE,0,&lPath,0,&luiUVIndex);

	std::string lacFilename = cFileUtils::GetFileName(lPath.data);
	std::string lDiffuserTextureFile = lpMaterialData->macPath+"/"+lacFilename;
	mDiffuseTexture = cTextureManager::Get().LoadResource( lDiffuserTextureFile, lDiffuserTextureFile);

  mbLoaded = true;
  return true;
*/
}
void cMaterial::PrepareRender()
{
	// Set the technique
	assert( mEffect.IsValidHandle() );
	cEffect * lpEffect = (cEffect *)mEffect.GetResource();
	assert( lpEffect );
	lpEffect->SetTechnique("Technique0");

	// Set Propierties
	cMatrix lWVPMatrix = cGraphicManager::Get().GetWVPMatrix();
	lpEffect->SetParam("WorldViewProj", lWVPMatrix );
	cCamera * lpCamera = cGraphicManager::Get().GetActiveCamera();
	cVec3 lCameraPos = lpCamera->GetView().GetPosition();
	lpEffect->SetParam("cameraPos", lCameraPos );
	lpEffect->SetParam("time", cGame::Get().GetAcumulatedTime() );

	// Set the textures
	for ( unsigned luiTextureIndex = 0; luiTextureIndex < maTextureData.size(); ++luiTextureIndex )
	{
		lpEffect->SetParam(
			maTextureData[luiTextureIndex].macShaderTextureID.c_str(),
			maTextureData[luiTextureIndex].mTexture);
	}
}
/*
void cMaterial::SetMaterial()
{
	//Set the diffuse texture
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	cTexture * lpTexture = (cTexture*)mDiffuseTexture.GetResource();
	unsigned luiTextureHandle = lpTexture->GetTextureHandle();
	glBindTexture( GL_TEXTURE_2D, luiTextureHandle );
}
*/

void cMaterial::ReadAllTextures(aiMaterial * lpAiMaterial, cMaterialData * lpMaterialData)
{
	// First pass counts the number of textures to allocate the memory
	unsigned luiTextureCount = 0;
	for (unsigned luiTextureTypeIndex = 0; kacTextureMapping[luiTextureTypeIndex].mTextureType != aiTextureType_NONE; ++luiTextureTypeIndex)
	{
		luiTextureCount += lpAiMaterial->GetTextureCount( kacTextureMapping[luiTextureTypeIndex].mTextureType);
	}

	// Exit if there is no texture
	if (luiTextureCount == 0)
	{
		return;
	}

	// Allocate the memory for the textures
	maTextureData.resize(luiTextureCount);

	// Initialize all the textures
	unsigned luiNextTexture = 0;
	for ( unsigned luiTextureTypeIndex = 0; kacTextureMapping[luiTextureTypeIndex].mTextureType != aiTextureType_NONE; ++luiTextureTypeIndex)
	{
		unsigned luiTextureCount = lpAiMaterial->GetTextureCount(kacTextureMapping[luiTextureTypeIndex].mTextureType);

		for ( unsigned luiTextureIndexInType = 0; luiTextureIndexInType < luiTextureCount; ++luiTextureIndexInType )
		{
			aiString lPath;
			unsigned luiUVIndex;
			lpAiMaterial->GetTexture( kacTextureMapping[luiTextureTypeIndex].mTextureType, luiTextureIndexInType,&lPath,0,&luiUVIndex);
			
			std::string lTextureFile = lpMaterialData->macPath + "/" + lPath.data;
			maTextureData[luiNextTexture].mTexture = cTextureManager::Get().LoadResource(lTextureFile, lTextureFile);
			assert(maTextureData[luiNextTexture].mTexture.IsValidHandle());

			std::stringstream lStream;
			lStream << kacTextureMapping[luiTextureTypeIndex].macPrefix << luiTextureIndexInType;
			maTextureData[luiNextTexture].macShaderTextureID = lStream.str();
			++luiNextTexture;
		}
	}
}

bool cMaterial::SetFirstPass()
{
   assert( mEffect.IsValidHandle() );
   cEffect * lpEffect = (cEffect *)mEffect.GetResource();
   assert( lpEffect );
   return lpEffect->SetFirstPass();
}     
bool cMaterial::SetNextPass()
{
   assert( mEffect.IsValidHandle() );
   cEffect * lpEffect = (cEffect *)mEffect.GetResource();
   assert( lpEffect );
   return lpEffect->SetNextPass();
}

//Deinit materials
void cMaterial::Deinit()
{;}

