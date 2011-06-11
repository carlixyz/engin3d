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
    cMaterial * lpMaterial = ( cMaterial * ) mMaterialHandles[luiIndex].GetResource();
	//Set the Mesh
	cMesh *lpMesh		   = (cMesh *) mMeshHandles[luiIndex].GetResource();
    
	// Prepare all the parameters for the render
	lpMaterial->PrepareRender();
	lpMesh->PrepareRender(mMaterialHandles[luiIndex]);

	// Set the first pass
	bool lbContinue = lpMaterial->SetFirstPass();

	while (lbContinue)
	{
		//Render Mesh
		lpMesh->RenderMesh();

		// Prepare the next Pass
		lbContinue = lpMaterial->SetNextPass();
	}
  }
}

void cObject::Update( float lfTimestep )
{
   for ( unsigned luiIndex = 0; luiIndex < mMeshHandles.size(); 
                                                         ++luiIndex )
   {
      cMesh *lpMesh = (cMesh *)mMeshHandles[luiIndex].GetResource();
      lpMesh->Update(lfTimestep);
   }
}
