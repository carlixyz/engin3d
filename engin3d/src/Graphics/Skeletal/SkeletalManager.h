#ifndef SKELETAL_MANAGER_H
#define SKELETAL_MANAGER_H

#include <string>
#include "../../Utility/ResourceManager.h"
#include "../../Utility/Singleton.h"

class cSkeletalMesh;
class cSkeletalManager :  public cResourceManager,
						  public cSingleton<cSkeletalManager>
{
  public:
    friend class cSingleton<cSkeletalManager>;

	virtual void Init( unsigned luiMaxSize );
	cSkeletalMesh * CreateSkeletalMesh( const std::string& lacCoreModelName );

  protected:
    cSkeletalManager() { ; }

  private:  
    virtual cResource * LoadResourceInternal( std::string lacNameID, const  std::string &lacFile );
};
#endif
