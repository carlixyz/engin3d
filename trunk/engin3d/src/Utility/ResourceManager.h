#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <vector>
#include <list>
#include "InternalResource.h"

class cResourceHandle; // Foward Declaration

class cResourceManager 
{
public:
	void Init( unsigned luiMaxSize );							// must assign 1 slot al least for start
	void Deinit();

	cResourceHandle SearchResource( std::string lacNameID );	// Find Resources loaded in Managers

	void UnloadResource( cResourceHandle * lpHandle );			
																
	cResourceHandle LoadResource( std::string lacNameID, const std::string &lacFile = "./Src/Data/XML/Config.xml" );
																// File Resources Loader ( by default loads from a Xml)
	cResourceHandle LoadResource( std::string lacNameID, void * lpMemoryData, int luiTypeID );
																// Memory Resources Loader
protected:	
	friend class cResourceHandle;
	// Attribs	
	int muiNextKey;												// Key assigned to the next Resource 
	unsigned muiMaxSize;										// Max Resources space available for Load

	std::vector<cInternalResource> maResources;
	std::list<unsigned> mFreeResourceSlot;

	cResourceHandle AddResourceToPool( cResource * lpResource );// Insert Resources in the vector

	//Implementations Relays on the derivated class and type of resource :
	virtual cResource * LoadResourceInternal( std::string lacNameID, const std::string &lacFile )
				{ return NULL; };								// Load Resources from Files 								
	virtual cResource * LoadResourceInternal( std::string lacNameID, void * lpMemoryData, int luiTypeID )
				{ return NULL; };								// Load Resources from memory

	cResource * GetResource( cResourceHandle *lpHandle );	

	std::string ReadXml(std::string lacNameID, const std::string &lacFile); 
};
#endif