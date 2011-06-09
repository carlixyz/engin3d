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

