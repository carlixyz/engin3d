#include "ResourceManager.h"
#include "ResourceHandle.h"


void cResourceManager::Init(unsigned int luiMaxSize)
{
	muiMaxSize = luiMaxSize;
	maResources.resize( muiMaxSize );
	mFreeResourceSlot.clear();

	for ( unsigned luiIndex = 0; luiIndex < muiMaxSize; ++luiIndex )
	{
		// Initialize the resources slot
		maResources[luiIndex].muiKey = kuiInvalidKey;
		maResources[luiIndex].mpResource = NULL;

		// Add the free index to the list
		mFreeResourceSlot.push_back( luiIndex );
	}

	// Prepare the first key
	muiNextKey = kuiInvalidKey + 1;
}

void cResourceManager::Deinit()
{
	for ( unsigned luiIndex = 0; luiIndex < muiMaxSize; ++luiIndex )
	{
		// Is a valid resource=
		if ( maResources[luiIndex].muiKey != kuiInvalidKey )
		{
			// Check that all is right
			assert( maResources[luiIndex].mpResource );
			assert( maResources[luiIndex].mpResource->IsLoaded() );

			// Deinit the resource
			maResources[luiIndex].mpResource->Deinit();
			delete maResources[luiIndex].mpResource;

			// Clear the resource slot
			maResources[luiIndex].muiKey = kuiInvalidKey;
			maResources[luiIndex].mpResource = NULL;
		}
	}
}

cResource * cResourceManager::GetResource(cResourceHandle *lpHandle)
{
	assert( lpHandle );
	assert( lpHandle->IsValidHandle() );

	unsigned luiIndex = lpHandle->GetID(); // assignate index ID 
	assert( luiIndex < muiMaxSize );

	if ( maResources[luiIndex].muiKey == lpHandle->GetKey()
		&& maResources[luiIndex].mpResource->IsLoaded() )
	{
		return maResources[luiIndex].mpResource;
	}
	return NULL;
}

cResourceHandle cResourceManager::FindResource(std::string lacNameID)
{
	cResourceHandle lHandle;
	for ( unsigned luiIndex = 0; luiIndex < muiMaxSize; ++luiIndex )
	{
		// Is a valid Resource ?
		if ( maResources[luiIndex].muiKey != kuiInvalidKey )
		{
			// Check that all is right
			assert( maResources[luiIndex].mpResource );
			assert( maResources[luiIndex].mpResource->IsLoaded() );

			// Is the right resource ?
			if ( maResources[luiIndex].mpResource->IsThisResource( lacNameID ) )
			{
				lHandle.Init(this, luiIndex, maResources[luiIndex].muiKey); // Init it
				break;
			}
		}
	}
	return lHandle;
}

void cResourceManager::UnloadResource(cResourceHandle *lpHandle)
{
	assert( lpHandle );
	assert( lpHandle->IsValidHandle() );

	unsigned luiIndex = lpHandle->GetID();
	assert( luiIndex < muiMaxSize );

	if ( maResources[luiIndex].muiKey == lpHandle->GetKey()
			&& maResources[luiIndex].mpResource->IsLoaded() )
	{
		// Deinit the resource
		maResources[luiIndex].mpResource->Deinit();

		// Free Space in memory
		delete maResources[luiIndex].mpResource;

		// Clear the resource slot
		maResources[luiIndex].muiKey = kuiInvalidKey;

		// Quit the Pointer target
		maResources[luiIndex].mpResource = NULL;

		// Add the slot to the Free List
		mFreeResourceSlot.push_front(luiIndex);
	}
}

cResourceHandle cResourceManager::LoadResource(std::string lacNameID, const std::string &lacFile)
{
	cResourceHandle lHandle = FindResource( lacNameID );
	if (!lHandle.IsValidHandle() )
	{
		// Load the resource fromt the File
		cResource * lpResource = LoadResourceInternal( lacNameID, lacFile );

		if (lpResource)
		{ 
			// Set the Id
			lpResource->SetNameID( lacNameID );
			// Save it to the pool
			lHandle = AddResourceToPool( lpResource );
		}
		else
		{
			lHandle.Invalidate();
		}
	}
	return lHandle;
}
cResourceHandle cResourceManager::LoadResource(std::string lacNameID, void * lpMemoryData, int luiTypeID )
{
	cResourceHandle lHandle = FindResource( lacNameID );
	if (!lHandle.IsValidHandle() )
	{
		// Load the resource From memory
		cResource * lpResource = LoadResourceInternal( lacNameID, lpMemoryData, luiTypeID );

		if (lpResource)
		{ 
			// Set the Id
			lpResource->SetNameID( lacNameID );
			// Save it to the pool
			lHandle = AddResourceToPool( lpResource );
		}
		else
		{
			lHandle.Invalidate();
		}
	}
	return lHandle;
}

cResourceHandle cResourceManager::AddResourceToPool(cResource *lpResource)
{
	assert( mFreeResourceSlot.size() > 0 );

	unsigned luiNext = *mFreeResourceSlot.begin();
	mFreeResourceSlot.pop_front();

	assert( maResources[luiNext].muiKey == kuiInvalidKey );
	assert( muiNextKey != kuiInvalidKey );

	maResources[luiNext].muiKey = muiNextKey++;
	maResources[luiNext].mpResource = lpResource;

	cResourceHandle lHandle;
	lHandle.Init(this, luiNext, maResources[luiNext].muiKey);

	return lHandle;
}


