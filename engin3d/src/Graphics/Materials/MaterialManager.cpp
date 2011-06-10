#include "MaterialManager.h"
#include "Material.h"

cResource * cMaterialManager::LoadResourceInternal( std::string lacNameId, void * lpMemoryData, int liTypeID)
{
  cMaterial *lpMaterial = new cMaterial();
  lpMaterial->Init(lacNameId, lpMemoryData, liTypeID );
  
  return lpMaterial;
}

cResource * cMaterialManager::LoadResourceInternal(std::string lacNameID, const std::string &lacFile)
{
  cMaterial *lpMaterial = new cMaterial();

  if (!lpMaterial->Init(lacNameID, lacFile))
  {
	  delete lpMaterial;
	  return NULL;
  }
  return lpMaterial;
}
  