#include "SkeletalMesh.h"
#include "../GLHeaders.h"
#include <assimp.hpp>
#include <aiScene.h>
#include <aiPostProcess.h>
#include <cassert>
#include "SkeletalCoreModel.h"
#include "../GraphicManager.h"
#include "../../MathLib/MathLib.h"
#include "../Materials/Material.h"
#include "../Effects/Effect.h"

bool cSkeletalMesh::Init(const std::string &lacNameID, void *lpMemoryData, int liDataType)
{
	cSkeletalCoreModel * lpCoreModel = (cSkeletalCoreModel *)lpMemoryData;
	lpCoreModel->CreateInstance(this);

	return true;
}

void cSkeletalMesh::Deinit()
{
	delete mpCal3DModel;
}

void cSkeletalMesh::Update(float lfTimeStep)
{
	mpCal3DModel->update(lfTimeStep);
}

bool cSkeletalMesh::PlayAnim(const std::string &lacAnimName, float lfWeight, float lfDelayIn, float lfDelayOut)
{
	// Search for the animation
	sAnimationDef* lpDef = mpCoreModel->GetAnimationDef(lacAnimName);
	if (lpDef)
	{
		if (lpDef->meAnimType == eAnimType_Cycle)
		{
			// blend the cycle
			mpCal3DModel->getMixer()->blendCycle( lpDef->miAnimID, lfWeight, lfDelayIn );
		}
		else
		{
			// It´s an action
			mpCal3DModel->getMixer()->executeAction( lpDef->miAnimID, lfDelayIn, lfDelayOut, lfWeight );
		}
		return true;
	}
	return false;
}

void cSkeletalMesh::StopAnim(const std::string &lacAnimName, float lfDelayOut)
{
	sAnimationDef *lpDef = mpCoreModel->GetAnimationDef(lacAnimName);
	if (lpDef)
	{
		if (lpDef->meAnimType == eAnimType_Cycle)
		{
			// blend the cycle
			mpCal3DModel->getMixer()->clearCycle(lpDef->miAnimID, lfDelayOut);
		}
		else
		{
			// It´s an action
			mpCal3DModel->getMixer()->removeAction(lpDef->miAnimID, lfDelayOut);
		}
	}
}

void cSkeletalMesh::RenderSkeleton(void)
{
	cMatrix lWorld;				// REALLY WEIRD MATRIX SET UP
	lWorld.LoadIdentity();

	lWorld.LoadScale(0.01f);	// SCALING BECAUSE ORIGINAL MODEL IT´S TOO BIG ( IN OTHER CASE: JUST QUIT IT )
	cGraphicManager::Get().SetWorldMatrix(lWorld);


	unsigned luiBoneCount = mpCal3DModel->getSkeleton()->getCoreSkeleton()->getVectorCoreBone().size();
	assert(luiBoneCount < 120);

	float mafLines[120] [6];
	int liBones = mpCal3DModel->getSkeleton()->getBoneLines(&mafLines[0][0]);
	for (int liIndex = 0; liIndex < liBones; ++liIndex)
	{
		cVec3 lvP1( mafLines[liIndex][0],
					mafLines[liIndex][1],
					mafLines[liIndex][2] );

		cVec3 lvP2( mafLines[liIndex][3],
					mafLines[liIndex][4],
					mafLines[liIndex][5] );

		cGraphicManager::Get().DrawLine( lvP1, lvP2, cVec3(1.0f, 1.0f, 1.0f) );
	}
	lWorld.LoadIdentity();
	cGraphicManager::Get().SetWorldMatrix(lWorld);
}
void cSkeletalMesh::RenderMesh()
{
   // Position
   glBindBuffer(GL_ARRAY_BUFFER, mpCoreModel->mVboVertices);
   assert(glGetError() == GL_NO_ERROR);
   glVertexPointer(3, GL_FLOAT, sizeof(float) * 3, 0);
   assert(glGetError() == GL_NO_ERROR);

   // Normals
   glBindBuffer(GL_ARRAY_BUFFER, mpCoreModel->mVboNormals);
   assert(glGetError() == GL_NO_ERROR);
   glNormalPointer(GL_FLOAT, sizeof(float) * 3, 0);
   assert(glGetError() == GL_NO_ERROR);
   
// This line has been added
//-----------------------------------------------------------------
// Set all the UV channels to the render
   glEnableClientState(GL_TEXTURE_COORD_ARRAY);
   assert(mpCoreModel->maVboTexture.size() <= 3);
   static GLenum meTextureChannelEnum[] = { GL_TEXTURE0, GL_TEXTURE1, GL_TEXTURE2, GL_TEXTURE3 };
   
   for(unsigned luiTexCoordChannel = 0; luiTexCoordChannel < mpCoreModel->maVboTexture.size(); ++luiTexCoordChannel)
   {
	   // Texture coordinates
	   glClientActiveTexture(meTextureChannelEnum[luiTexCoordChannel]);
	   glBindBuffer(GL_ARRAY_BUFFER, mpCoreModel->maVboTexture[luiTexCoordChannel]);
	   assert(glGetError() == GL_NO_ERROR);

	   glTexCoordPointer(2, GL_FLOAT, sizeof(float)*2, 0);
	   assert(glGetError() == GL_NO_ERROR); 
   }

   // Bone Index
   glClientActiveTexture( meTextureChannelEnum[mpCoreModel->maVboTexture.size()]);
   glBindBuffer(GL_ARRAY_BUFFER, mpCoreModel->mVboBoneIx);
   assert(glGetError() == GL_NO_ERROR);
   glTexCoordPointer(4, GL_FLOAT, sizeof(float)*4, 0);
   assert(glGetError() == GL_NO_ERROR); 

   // Weights
   glBindBuffer(GL_ARRAY_BUFFER, mpCoreModel->mVboWeight);
   assert(glGetError() == GL_NO_ERROR);
   glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(unsigned char) * 4, 0);
   assert(glGetError() == GL_NO_ERROR);

   // Index
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mpCoreModel->mVboIndex);
   assert(glGetError() == GL_NO_ERROR);

   glEnableClientState(GL_VERTEX_ARRAY);
   glEnableClientState(GL_NORMAL_ARRAY);
   glEnableClientState(GL_COLOR_ARRAY);
   glEnableClientState(GL_TEXTURE_COORD_ARRAY);
   glDrawRangeElements(GL_TRIANGLES, 0, mpCoreModel->muiIndexCount, mpCoreModel->muiIndexCount, GL_UNSIGNED_INT, NULL);

   assert(glGetError() == GL_NO_ERROR);

   glDisableClientState(GL_COLOR_ARRAY);
   glDisableClientState(GL_VERTEX_ARRAY);
   glDisableClientState(GL_NORMAL_ARRAY);
   glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}


