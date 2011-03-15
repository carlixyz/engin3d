#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include "../../Utility/ResourceManager.h"
#include "../../Utility/Singleton.h"
#include "Font.h"


class cFontManager : public cResourceManager,
							public cSingleton<cFontManager>
{
	public:
		friend class cSingleton<cFontManager>;
	
	protected:
		cFontManager() { ; }// Protected Constructor

	private:
		virtual cResource* LoadResourceInternal( std::string lacNameID,
												const std::string &lacFile );
};

#endif 