#include "Scene.h"

#include "../../Graphics/Meshes/assimp/Include/assimp.hpp"
#include "../../Graphics/Meshes/assimp/Include/aiScene.h"
#include "../../Graphics/Meshes/assimp/Include/aiPostProcess.h"

//#include <assimp.hpp>	// we have already included this files in the project
//#include <aiScene.h>	// so why don´t use them...
//#include <aiPostProcess.h>

#include <cassert>
#include "../../Graphics/Meshes/MeshManager.h"
#include "../../Graphics/Meshes/Mesh.h"
#include "../../Graphics/Materials/MaterialData.h"
#include "../../Graphics/Materials/MaterialManager.h"
#include "../../Utility/FileUtils.h"


//Init 
bool cScene::Init(const std::string &lacNameId, const std::string &lacFile)
{
  macFile = lacFile;
  mbLoaded = false;

  //Create an instance of the importer class
  Assimp::Importer lImporter;

  //Load the scene
  const aiScene* lpScene = lImporter.ReadFile( lacFile.c_str(),
    aiProcess_CalcTangentSpace |
    aiProcess_Triangulate      |
    aiProcess_JoinIdenticalVertices |
    aiProcess_SortByPType);

  // If the import failed, report it
  if (!lpScene)
  {
    printf( lImporter.GetErrorString() );
    return false;
  }

  
  ProcessScene(lpScene);
  lImporter.FreeScene();
  mbLoaded = true;
  return true;
}

//Deinit
void cScene::Deinit()
{
	  cMaterialManager::Get().Deinit();
	  
	  cMeshManager::Get().Deinit();
}

//ProcessScene
void cScene::ProcessScene( const aiScene* lpScene )
{
  cMeshManager::Get().Init(lpScene->mNumMeshes);
  for (unsigned luiIndex = 0; luiIndex< lpScene->mNumMeshes; ++luiIndex)
  {
    char lacMeshName[512];
    sprintf( lacMeshName, "%s_%d", macFile.c_str(), luiIndex);
    cResourceHandle lHandle;
    lHandle = cMeshManager::Get().LoadResource(lacMeshName, 
      lpScene->mMeshes[luiIndex], 0);
    mMeshList.push_back(lHandle);
	int liMaterialIndex = lpScene->mMeshes[luiIndex]->mMaterialIndex;
	mMeshMaterialIndexList.push_back(liMaterialIndex);
  }

  //Materials
  assert( lpScene->HasMaterials());

  cMaterialManager::Get().Init(lpScene->mNumMaterials);

  for (unsigned luiIndex = 0; luiIndex < lpScene->mNumMaterials; ++luiIndex )
  {
	  // Access the material name
	  aiString lName;
	  lpScene->mMaterials[luiIndex]->Get(AI_MATKEY_NAME, lName);

	  // Fill in the material data structure
	  cMaterialData lMaterialData;
	  lMaterialData.macPath = cFileUtils::GetDirectory(macFile);
	  lMaterialData.mpMaterial = lpScene->mMaterials[luiIndex];

	  // Load the resource
	  cResourceHandle lHandle;
	  lHandle = cMaterialManager::Get().LoadResource(lName.data, &lMaterialData, 0);

	  // Save the material on a vector in the Scene
	  mMaterialList.push_back(lHandle);
  }
}

//Render scene
void cScene::Render(void)
{
	for ( unsigned luiIndex = 0; luiIndex < mMeshList.size(); ++luiIndex)
	{
	// Get material index
	unsigned luiMaterialIndex  = mMeshMaterialIndexList[luiIndex];

	// Set the material 
	void * lpResource = mMaterialList[luiMaterialIndex].GetResource();
	((cMesh *)lpResource)->RenderMesh();
	}

}