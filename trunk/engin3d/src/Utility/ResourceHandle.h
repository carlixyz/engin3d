#ifndef RESOURCE_HANDLE_H
#define RESOURCE_HANDLE_H

#include <stdlib.h>
#include "ResourceManager.h"

class cResourceHandle 
{
public:
	cResourceHandle() { mpManager = NULL; muiKey = kuiInvalidKey; }// Constructor

	cResource * GetResource();	// Getter
	
	inline unsigned GetID()		{ return muiID; }	// Return Resource ID

	inline unsigned GetKey()	{ return muiKey; }  // Return Resource Key "Pass"

	inline bool IsValidHandle()	{ return muiKey != kuiInvalidKey; } // Check in

	inline void Invalidate()	{ muiKey = kuiInvalidKey; } // Setter to Free one Resource

protected:
	
	friend class cResourceManager;					

	void Init( cResourceManager * lpManager, 
				unsigned luiID,
				unsigned luiKey );

	cResourceManager * mpManager;					// Variable Resource type
	unsigned int muiID;								// Id Number of Index
	unsigned int muiKey;							// Key 

};

#endif