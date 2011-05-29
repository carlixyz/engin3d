#include "Scene.h"

#include <assimp.hpp>
#include <aiScene.h>
#include <aiPostProcess.h>

#include <cassert>

#include "../../Graphics/Meshes/MeshManager.h"
#include "../../Graphics/Meshes/Mesh.h"
#include "../../Graphics/Materials/MaterialManager.h"
#include "../../Graphics/Materials/MaterialData.h"
#include "../../Utility/FileUtils.h"

#include <iostream>

//Init 
bool cScene::Init(const std::string &lacNameId, const std::string &lacFile)
{

  macFile = lacFile;
  mbLoaded = false;

  //Create an instance of the importer class
  Assimp::Importer lImporter;

  //Load the scene
  const aiScene* lpScene = lImporter.ReadFile( macFile.c_str(),
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
  for(unsigned luiIndex=0; luiIndex < mObjectList.size(); ++luiIndex)
  {
    mObjectList[luiIndex]->Deinit();
    delete mObjectList[luiIndex];
  }

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
    lHandle = cMeshManager::Get().LoadResource( lacMeshName, lpScene->mMeshes[luiIndex], 0);
    mMeshList.push_back(lHandle);
	int liMaterialIndex = lpScene->mMeshes[luiIndex]->mMaterialIndex;
	mMeshMaterialIndexList.push_back(liMaterialIndex);
  }

  //Materials
  if ( lpScene->HasMaterials())
  {

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

  
  //conver nodes to objects
  if (lpScene->mRootNode)
  {
    cMatrix lMatrix;
    lMatrix.LoadIdentity();
    ConvertNodesToObjects(lpScene->mRootNode, lMatrix);
  }
  
}

//Render scene
void cScene::Render(void)
{
  for(unsigned luiIndex = 0; luiIndex < mObjectList.size();++luiIndex)
  {
    mObjectList[luiIndex]->Render();
  }

}

//Plain the hierarchy
void cScene::ConvertNodesToObjects( aiNode * lpNode, cMatrix lTransform)
{
  // Create a new scene object for the node if this has meshes
  cMatrix lNodeTransform( cVec4 ( lpNode->mTransformation.a1,
    lpNode->mTransformation.b1,
    lpNode->mTransformation.c1,
    lpNode->mTransformation.d1),
    cVec4 ( lpNode->mTransformation.a2,
    lpNode->mTransformation.b2,
    lpNode->mTransformation.c2,
    lpNode->mTransformation.d2),
    cVec4 ( lpNode->mTransformation.a3,
    lpNode->mTransformation.b3,
    lpNode->mTransformation.c3,
    lpNode->mTransformation.d3),
    cVec4 ( lpNode->mTransformation.a4,
    lpNode->mTransformation.b4,
    lpNode->mTransformation.c4,
    lpNode->mTransformation.d4));

  lTransform = lNodeTransform * lTransform;

  if ( lpNode->mNumMeshes > 0)
  {
    cObject *lpObject = new cObject;
    lpObject->Init();
    lpObject->SetName( lpNode->mName.data );
    lpObject->SetWorldMatrix(lTransform);

    for (unsigned luiIndex=0; luiIndex<lpNode->mNumMeshes; ++luiIndex)
    {
      unsigned luiMeshIndex = lpNode->mMeshes[luiIndex];
      unsigned luiMaterialIndex;
      luiMaterialIndex = mMeshMaterialIndexList[luiMeshIndex];
      lpObject->AddMesh(mMeshList[luiMeshIndex], 
        mMaterialList[luiMaterialIndex]);
    }
    mObjectList.push_back(lpObject);
  }

  //Recursive call
  for(unsigned luiIndex = 0; luiIndex<lpNode->mNumChildren; ++luiIndex)
  {
    ConvertNodesToObjects(lpNode->mChildren[luiIndex], lTransform);
  }

}

//Update scene
void cScene::Update(float lfTimeStep)
{
  for(unsigned luiIndex=0; luiIndex<mObjectList.size();++luiIndex)
  {
    mObjectList[luiIndex]->Update(lfTimeStep);
  }
}