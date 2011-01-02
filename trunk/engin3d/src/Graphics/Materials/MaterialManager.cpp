#include "MaterialManager.h"
#include "Material.h"

cResource * cMaterialManager::LoadResourceInternal( std::string lacNameId, 
                                               void * lpMemoryData,
                                               int liTypeID)
{
  cMaterial *lpMaterial = new cMaterial();
  lpMaterial->Init(lacNameId, lpMemoryData, liTypeID );
  
  return lpMaterial;
}

