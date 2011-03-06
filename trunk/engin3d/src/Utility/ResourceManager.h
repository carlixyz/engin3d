#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <vector>
#include <list>
#include "InternalResource.h"

class cResourceHandle; // Foward Declaration

class cResourceManager 
{
public:
	void Init( unsigned luiMaxSize );
	void Deinit();


	cResourceHandle FindResource( std::string lacNameID ); 

	void UnloadResource( cResourceHandle * lpHandle );			
																// File Resources Loader
	cResourceHandle LoadResource( std::string lacNameID, const std::string &lacFile );
																// Memory Resources Loader
	cResourceHandle LoadResource( std::string lacNameID, void * lpMemoryData, int luiTypeID );

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