#include "MeshManager.h"
#include "Mesh.h"

cResource * cMeshManager::LoadResourceInternal( std::string lacNameId, 
                                               void * lpMemoryData,
                                               int luiTypeID)
{
  cMesh *lpMesh = new cMesh();
  if (!lpMesh->Init(lacNameId, lpMemoryData,luiTypeID))
  {
    delete lpMesh;
    return NULL;
  }

  return lpMesh;
}

