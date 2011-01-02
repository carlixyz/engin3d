#include "Mesh.h"

#include "../GLHeaders.h"

#include "../../Graphics/Meshes/assimp/Include/assimp.hpp"
#include "../../Graphics/Meshes/assimp/Include/aiScene.h"
#include "../../Graphics/Meshes/assimp/Include/aiPostProcess.h"

//#include <assimp.hpp>	// we have already included this files in the project
//#include <aiScene.h>	// so why don´t use them ?...
//#include <aiPostProcess.h>

#include <cassert>

//Init mesh into memory
bool cMesh::Init( const std::string &lacNameID, 
                 void * lpMemoryData, int luiTypeID)
{
  mbLoaded = false;
  aiMesh * lpAiMesh = (aiMesh*) lpMemoryData;

  //Create vertex buffer
  glGenBuffers(1, &mVboVertices);
  assert(glGetError() == GL_NO_ERROR);

  glBindBuffer(GL_ARRAY_BUFFER, mVboVertices);
  assert(glGetError() == GL_NO_ERROR);

  glBufferData(GL_ARRAY_BUFFER, 
    sizeof(float) * 3 * lpAiMesh->mNumVertices,
    lpAiMesh->mVertices, GL_STATIC_DRAW);
  assert(glGetError() == GL_NO_ERROR);

  //Create normal buffer
  glGenBuffers(1, &mVboNormals);
  assert(glGetError() == GL_NO_ERROR);

  //Check that the mesh has normals
  mbHasNormals = false;
  if (lpAiMesh->HasNormals())
  {
    mbHasNormals = true;
    glBindBuffer(GL_ARRAY_BUFFER, mVboNormals);
    assert(glGetError() == GL_NO_ERROR);
  
    glBufferData(GL_ARRAY_BUFFER, 
      sizeof(float) * 3 * lpAiMesh->mNumVertices,
      lpAiMesh->mNormals, GL_STATIC_DRAW);
    assert(glGetError() == GL_NO_ERROR);
  }

  //Create texture buffer
  mbHasTexture = false;
  if (lpAiMesh->HasTextureCoords(0))
  {
    mbHasTexture = true;
    unsigned luiTexCoordNum = lpAiMesh->mNumVertices;
    unsigned luiTexCoordFloats = 2 * luiTexCoordNum;
    float * lpTexCoordinates = new float[ luiTexCoordFloats ];
    unsigned luiIndex = 0;
    for ( unsigned luiTexIndex = 0; luiTexIndex < luiTexCoordNum; ++luiTexIndex)
    {
      lpTexCoordinates[luiIndex++] = lpAiMesh->mTextureCoords[0][luiTexIndex].x;
      lpTexCoordinates[luiIndex++] = (1.0f - lpAiMesh->mTextureCoords[0][luiTexIndex].y);
    }

    glGenBuffers(1, &mVboTexture);
    assert(glGetError() == GL_NO_ERROR);
  
    glBindBuffer(GL_ARRAY_BUFFER, mVboTexture);
    assert(glGetError() == GL_NO_ERROR);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * luiTexCoordFloats,
      lpTexCoordinates, GL_STATIC_DRAW);
    assert(glGetError() == GL_NO_ERROR);

    delete [] lpTexCoordinates;
  }

  //Create Index buffer
  muiIndexCount = lpAiMesh->mNumFaces * 3;
  unsigned * lpIndex = new unsigned [ muiIndexCount ];
  unsigned luiIndex = 0;
  for (unsigned luiFaceIndex = 0; luiFaceIndex < lpAiMesh->mNumFaces; ++luiFaceIndex)
  {
    //Make sure that each face has 3 vertex
    assert(lpAiMesh->mFaces[luiFaceIndex].mNumIndices == 3 );

    //Assign vertex to index structure
    lpIndex[luiIndex++] = lpAiMesh->mFaces[luiFaceIndex].mIndices[0];
    lpIndex[luiIndex++] = lpAiMesh->mFaces[luiFaceIndex].mIndices[1];
    lpIndex[luiIndex++] = lpAiMesh->mFaces[luiFaceIndex].mIndices[2];
  }

  glGenBuffers(1, &mVboIndex);
  assert(glGetError() == GL_NO_ERROR);
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboIndex);
  assert(glGetError() == GL_NO_ERROR);

  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * muiIndexCount,
    lpIndex, GL_STATIC_DRAW);
  assert(glGetError() == GL_NO_ERROR);
  delete [] lpIndex;

  //End of the method with the object in memory
  mbLoaded = true;
  return true;
} 


//Deinit mesh
void cMesh::Deinit()
{
  glDeleteBuffers(1, &mVboVertices);
  glDeleteBuffers(1, &mVboTexture);
  glDeleteBuffers(1, &mVboNormals);
  glDeleteBuffers(1, &mVboIndex);
}

//Render mesh
void cMesh::RenderMesh()
{
  glColor3f(1.0f, 1.0f, 1.0f);

  //Vertex buffer
  glBindBuffer(GL_ARRAY_BUFFER, mVboVertices);
  assert(glGetError() == GL_NO_ERROR);

  glVertexPointer(3, GL_FLOAT, sizeof(float)*3, 0);
  assert(glGetError() == GL_NO_ERROR);

  //Normal buffer
  if (mbHasNormals)
  {
    glBindBuffer(GL_ARRAY_BUFFER, mVboNormals);
    assert(glGetError() == GL_NO_ERROR);
    glNormalPointer(GL_FLOAT, sizeof(float) * 3, 0);
    assert(glGetError() == GL_NO_ERROR);
  }

  if (mbHasTexture)
  {
    glBindBuffer(GL_ARRAY_BUFFER, mVboTexture);
    assert(glGetError() == GL_NO_ERROR);
    glTexCoordPointer(2, GL_FLOAT, sizeof(float)*2, 0);
    assert(glGetError() == GL_NO_ERROR);
  }

  //Index buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboIndex);
  assert(glGetError() == GL_NO_ERROR);

  //Activate buffers in OpenGL state machine 
  glEnableClientState(GL_VERTEX_ARRAY);
  if (mbHasNormals) glEnableClientState(GL_NORMAL_ARRAY);
  if (mbHasTexture) glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  //Draw the buffers
  glDrawRangeElements(GL_TRIANGLES, 
    0, 
    muiIndexCount,
    muiIndexCount,
    GL_UNSIGNED_INT,
    NULL);
  assert(glGetError() == GL_NO_ERROR);

  //Deactivate buffers
  glDisableClientState(GL_VERTEX_ARRAY);
  if (mbHasNormals) glDisableClientState(GL_NORMAL_ARRAY);
  if (mbHasTexture) glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}