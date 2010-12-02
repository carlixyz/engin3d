
#include "ResourceHandle.h"


void cResourceHandle::Init( cResourceManager * lpManager, unsigned luiID, unsigned luiKey )
{
	mpManager = lpManager;
	muiID = luiID;
	muiKey = luiKey;
}

cResource * cResourceHandle::GetResource()
{
	if ( muiKey != kuiInvalidKey )
	{
		return mpManager->GetResource(this);
	}
	else
	{
		return NULL;
	}
}
