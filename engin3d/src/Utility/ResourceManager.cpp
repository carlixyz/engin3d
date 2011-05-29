#include "ResourceManager.h"

#include <cassert>
#include "ResourceHandle.h"

#include <tinystr.h>
#include <tinyxml.h>


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

cResourceHandle cResourceManager::SearchResource(std::string lacNameID)
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
	cResourceHandle lHandle = SearchResource( lacNameID );
	if (!lHandle.IsValidHandle() )
	{
		std::string lacPath = ReadXml( lacNameID, lacFile);//check Load from a XML

		// Load the resource from the File
		cResource * lpResource = LoadResourceInternal( lacNameID, lacPath );

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
	cResourceHandle lHandle = SearchResource( lacNameID );
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
	assert( mFreeResourceSlot.size() > 0 ); //  you must Init at least one slot

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

std::string cResourceManager::ReadXml(std::string lacNameID, const std::string &lacPath)
{

	if ( lacPath.substr(lacPath.find_last_of(".") + 1) == "xml"  ) // Check if it has a ".xml" extension
	{
		TiXmlDocument doc( lacPath );
	  
		if ( doc.LoadFile() )  // If xml document큦 loaded 
		{
			TiXmlHandle docHandle( &doc );
			TiXmlElement *pElem ;
			TiXmlAttribute* pAttrib; 
	
			pElem = docHandle.FirstChildElement("Application").FirstChildElement("Resources").FirstChildElement().ToElement();
	 
		 // seeking in: <DOC> / <Application> / <Resources> / <Resource(Attribs: name = "someId" file = "somePath")> 

			while ( pElem )
			{
				pAttrib = pElem->FirstAttribute();

				if ( pAttrib->ValueStr() == lacNameID )// assert ( pAttrib->NameTStr() == "name" )
			 	 	return	(pAttrib->Next()->ValueStr());// assert ( pAttrib->Next()->NameTStr() == "file" )
				else
					pElem = pElem->NextSiblingElement();// else keep seeking the next Element..

			} 	// or else It means that there is something wrong inside XML or the folder큦 path address	
			
		}	return "./Src/Data/Scene/fail.DAE"; // Return default Resource 큦 path

	}	return lacPath; // or else it큦 just a diferent extension (.dae, .dds, .jpg, etc)
}
