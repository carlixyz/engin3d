#include "FontManager.h"
#include "Font.h"


cResource * cFontManager::LoadResourceInternal( std::string lacNameID, const std::string &lacFile )
{
	//Create a texture
	cFont* lpFont = new cFont();
	
	//Load texture //    mFont.Init( "./Src/Data/Fonts/Test1.fnt" );
	// mFontHandle = cFontManager::Get().LoadResource("Font1", "./Data/Fonts/Test1.fnt");
	if (!lpFont->Init( lacNameID, lacFile ) )
	{
		delete lpFont;
		return NULL;
	}

	//Return texture as resource
	return lpFont;
}