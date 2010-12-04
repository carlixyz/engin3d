#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>

class cResource // Base class with polimorfic functions
{
public:
	virtual bool Init( const std::string &lacNameID, // File source Resource
			const std::string &lacFile ){ return false; }

	virtual bool Init( const std::string &lacNameID, // Memory source Resource
			void * lacMemoryData )		{ return false; }

	virtual void Deinit()				{ ; }		
	
	virtual bool IsLoaded()				{ return false; }

	virtual void Reload()				{ ; }

	bool IsThisResource( const std::string &lacNameID )
										{ return macNameID == lacNameID; }

	inline void SetNameID( const std::string &lacNameID )     // Setter
										{ macNameID = lacNameID; }

	inline std::string GetNameID()		{ return macNameID; } // Getter

private:
	std::string macNameID;

};

#endif