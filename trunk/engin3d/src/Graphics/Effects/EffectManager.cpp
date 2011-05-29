#include "EffectManager.h"
#include "Effect.h"

cEffectManager::cEffectManager()
{
	mCGContext = cgCreateContext();
	cgGLRegisterStates(mCGContext);
	cgGLSetManageTextureParameters(mCGContext, CG_TRUE);
}

cResource *cEffectManager::LoadResourceInternal( std::string lacNameID, const std::string &lacFile )
{
	cEffect * lpEffect = new cEffect();
	if ( !lpEffect->Init( lacNameID, lacFile ) )
	{
		delete lpEffect;
		return NULL;
	}
	return lpEffect;
}
void cEffectManager::Reload()
{
	for (unsigned luiIndex = 0; luiIndex < muiMaxSize; ++luiIndex)
	{
		// Is a valid Source?
		if ( maResources[luiIndex].muiKey != kuiInvalidKey )
		{
			//Check that all is right
			assert( maResources[luiIndex].mpResource );
			assert( maResources[luiIndex].mpResource->IsLoaded() );
			//Reload the resource
			maResources[luiIndex].mpResource->Reload();
		}
	}
}