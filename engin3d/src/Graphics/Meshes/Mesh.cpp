#include "Mesh.h"

#include "../GLHeaders.h"

#include <assimp.hpp>
#include <aiScene.h>
#include <aiPostProcess.h>

#include <cassert>

//Init mesh into memory
bool cMesh::Init( const std::string &lacNameID,void * lpMemoryData, int luiTypeID)
{
	std::string macFile = "";
  aiMesh * lpAiMesh = (aiMesh*) lpMemoryData;

  mbLoaded = false;

  //These lines have been added
  //-----------------------------------------------------------------------------------
  // Get the number of texture coordinates
  unsigned luiTextureCoordinateCount = lpAiMesh->GetNumUVChannels();

  assert(luiTextureCoordinateCount);
  maVboTexture.resize(luiTextureCoordinateCount);
  //-----------------------------------------------------------------------------------

   //Create vertex buffer
  glGenBuffers(1, &mVboVertices);
  assert(glGetError() == GL_NO_ERROR);

  //These lines have been added
  //-----------------------------------------------------------------------------------
  // Creating all the texture coordinate buffers
  for (unsigned luiIndex = 0; luiIndex < luiTextureCoordinateCount; ++luiIndex)
  {
	  glGenBuffers(1, &maVboTexture[luiIndex]);
	  assert(glGetError() == GL_NO_ERROR);
  }
  //-----------------------------------------------------------------------------------
  //Create normal buffer
  glGenBuffers(1, &mVboNormals);
  assert(glGetError() == GL_NO_ERROR);

    //Create normal buffer
  glGenBuffers(1, &mVboIndex);
  assert(glGetError() == GL_NO_ERROR);

	// Position
  glBindBuffer(GL_ARRAY_BUFFER, mVboVertices);
  assert(glGetError() == GL_NO_ERROR);
  glBufferData(GL_ARRAY_BUFFER, 
	  sizeof(float) * 3 * lpAiMesh->mNumVertices,
    lpAiMesh->mVertices, GL_STATIC_DRAW);
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

  // Tex Coords
  //These lines have been added
  //-------------------------------------------------------------------
  // Reading all the texture coordinate
  unsigned luiTexCoordNum = lpAiMesh->mNumVertices;
  unsigned luiTexCoordFloats = 2 * luiTexCoordNum;
  float *lpTexCoordinates = new float[ luiTexCoordFloats ];

  for(unsigned luiTexCoordChannel = 0; luiTexCoordChannel < luiTextureCoordinateCount; ++luiTexCoordChannel)
  {
	  unsigned luiInc = 0;
	  for ( unsigned luiTexIndex = 0; luiTexIndex < luiTexCoordNum; ++luiTexIndex )
	  {
		  lpTexCoordinates[luiInc++] =
			  lpAiMesh->mTextureCoords[luiTexCoordChannel][luiTexIndex].x;
		  lpTexCoordinates[luiInc++] = (1.0f -
			  lpAiMesh->mTextureCoords[luiTexCoordChannel][luiTexIndex].y);
		  // OpenGL Correction
	  }
	  glBindBuffer(GL_ARRAY_BUFFER, maVboTexture[luiTexCoordChannel]);
	  assert(glGetError() == GL_NO_ERROR);
	  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * luiTexCoordFloats,
		  lpTexCoordinates, GL_STATIC_DRAW);
	  assert(glGetError() == GL_NO_ERROR);
  }
  delete [] lpTexCoordinates;
  //------------------------------------------------------------------------

   // Index
   muiIndexCount = lpAiMesh->mNumFaces * 3;
   unsigned* lpIndex = new unsigned[ muiIndexCount ];

   unsigned luiIndex = 0;
   for ( unsigned luiFaceIndex = 0;
   luiFaceIndex < lpAiMesh->mNumFaces; ++luiFaceIndex )
   {
      assert(lpAiMesh->mFaces[luiFaceIndex].mNumIndices == 3 );
      lpIndex[luiIndex++]= lpAiMesh->mFaces[luiFaceIndex].mIndices[0];
      lpIndex[luiIndex++]= lpAiMesh->mFaces[luiFaceIndex].mIndices[1];
      lpIndex[luiIndex++]= lpAiMesh->mFaces[luiFaceIndex].mIndices[2];
   }
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboIndex);
   assert(glGetError() == GL_NO_ERROR);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, muiIndexCount * 
    sizeof(unsigned), lpIndex, GL_STATIC_DRAW);
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
  // These line has been  Added 
  // --------------------------------------------------------------
  for (unsigned luiTexCoordChannel = 0;
	  luiTexCoordChannel < maVboTexture.size();
	  ++luiTexCoordChannel)
  {
  glDeleteBuffers(1, &maVboTexture[luiTexCoordChannel]);
  }
  //---------------------------------------------------------------
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

     // This line has been added
   //-----------------------------------------------------------------
   // Set all the UV channels to the render
   glEnableClientState(GL_TEXTURE_COORD_ARRAY);
   static GLenum meTextureChannelEnum[] = { GL_TEXTURE0, GL_TEXTURE1, GL_TEXTURE2, GL_TEXTURE3 };
   for(unsigned luiTexCoordChannel = 0; luiTexCoordChannel < maVboTexture.size(); ++luiTexCoordChannel)
   {
   // Texture coordinates
      glClientActiveTexture(meTextureChannelEnum[luiTexCoordChannel]);
	  glBindBuffer(GL_ARRAY_BUFFER, maVboTexture[luiTexCoordChannel]);
	  assert(glGetError() == GL_NO_ERROR);
	  glTexCoordPointer(2, GL_FLOAT, sizeof(float)*2, 0);
	  assert(glGetError() == GL_NO_ERROR); 
   }
   // Index
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboIndex);
   assert(glGetError() == GL_NO_ERROR);
   glEnableClientState(GL_VERTEX_ARRAY);
   glEnableClientState(GL_NORMAL_ARRAY);
   glEnableClientState(GL_TEXTURE_COORD_ARRAY);
   glDrawRangeElements(GL_TRIANGLES, 
                       0, 
                       muiIndexCount, 
                       muiIndexCount, 
                       GL_UNSIGNED_INT, 
                       NULL);

   assert(glGetError() == GL_NO_ERROR);
   glDisableClientState(GL_VERTEX_ARRAY);
   glDisableClientState(GL_NORMAL_ARRAY);
   glDisableClientState(GL_TEXTURE_COORD_ARRAY);
///////////////////////////////////////////////////////////////////////
 /* if (mbHasTexture)
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
  */
}