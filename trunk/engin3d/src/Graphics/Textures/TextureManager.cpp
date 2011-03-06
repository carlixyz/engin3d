#include "TextureManager.h"
#include "Texture.h"


cResource * cTextureManager::LoadResourceInternal( std::string lacNameID, const std::string &lacFile )
{
	//Create a texture
	cTexture* lpTexture = new cTexture();
	
	//Load texture
	if (!lpTexture->Init( lacNameID, lacFile ) )
	{
		delete lpTexture;
		return NULL;
	}

	//Return texture as resource
	return lpTexture;
}