#include "Object.h"
#include "../../Graphics/GraphicManager.h"
#include "../../Graphics/Materials/Material.h"
#include "../../Graphics/Meshes/Mesh.h"

//Init method
void cObject::Init()
{
  macName = "";
  mWorldMatrix.LoadIdentity();
  mMeshHandles.resize(0);
  mMaterialHandles.resize(0);
}

//Add mesh to the object
void cObject::AddMesh( cResourceHandle lMeshHandle, cResourceHandle lMaterialHandle )
{
  mMeshHandles.push_back( lMeshHandle );
  mMaterialHandles.push_back( lMaterialHandle );
}

//Render method
void cObject::Render()
{
  //Set the world matrix
  cGraphicManager::Get().SetWorldMatrix(mWorldMatrix);
  for (unsigned luiIndex = 0; luiIndex < mMeshHandles.size(); ++luiIndex)
  {
    //Set the material
    cResource * lpResource;
    lpResource = mMaterialHandles[luiIndex].GetResource();
    assert(lpResource);
    ((cMaterial*)lpResource)->SetMaterial();

    //Render Mesh
    lpResource = mMeshHandles[luiIndex].GetResource();
    assert(lpResource);
    ((cMesh *)lpResource)->RenderMesh();
  }
}
