#include "SkeletalCoreModel.h"
#include "SkeletalMesh.h"
#include "../../MathLib/MathLib.h"
#include "../../Utility/FileUtils.h"
#include <tinystr.h>
#include <tinyxml.h>
#include <cassert>
#include "../GLHeaders.h"

//#include "SkeletalManager.h"

//#include "SkeletalMesh.h"

bool cSkeletalCoreModel::Init(const std::string &lacNameID, const std::string &lacFile)
{
	macFile = lacFile;
	std::string lacBaseDirectory = cFileUtils::GetDirectory(lacFile);
	// Load the XML file
	TiXmlDocument doc( lacFile.c_str() );
	bool lbLoadOk = doc.LoadFile();
	if ( lbLoadOk )
	{
		OutputDebugString("SkeletalCoreModel - XML Load: OK\n");
	}
	else
	{
		OutputDebugString("SkeletalCoreModel - XML Load: FAILED\n");
		return false;
	}

	TiXmlHandle lhDoc(&doc);
	TiXmlElement *lpElem;
	TiXmlHandle lhRoot(0);

	lpElem = lhDoc.FirstChildElement().Element();
	assert(lpElem);

	//Save this for later
	lhRoot = TiXmlHandle(lpElem);

	//Create the Cal3d Coremodel
	mpCoreModel = new CalCoreModel(lacNameID);
	// Obtain the Skeleton Path
	std::string lacSkeletonFile = lacBaseDirectory + lhRoot.ToElement()->Attribute("skeletonfile");

	// Load the skeleton
	bool lbIsOk = mpCoreModel->loadCoreSkeleton(lacSkeletonFile);
	assert(lbIsOk);

	//Read all the animations
	lpElem = lhRoot.FirstChild( "Animation" ).Element();
	for( lpElem; lpElem; lpElem = lpElem->NextSiblingElement("Animation") )
	{
		sAnimationDef lDefinition;
		lDefinition.macAnimationFile = lacBaseDirectory + lpElem->Attribute("file");
		lDefinition.macName = lpElem->Attribute("name");
		std::string lacType = lpElem->Attribute("type");

		if (lacType.length() > 0 && (lacType[0] == 'A' || lacType[0] == 'a' ) )
		{
			// it´s an action
			lDefinition.meAnimType = eAnimType_Action;
		}
		else
		{
			// It´s a Cycle
			lDefinition.meAnimType = eAnimType_Cycle;
		}
		// Load the animation in the coremodel
		lDefinition.miAnimID = mpCoreModel->loadCoreAnimation( lDefinition.macAnimationFile, lDefinition.macName );

		assert( lDefinition.miAnimID >= 0 );

		// Store the animation info for later
		mAnimationDefs.push_back(lDefinition);
	}
	// Read all the meshes
	lpElem = lhRoot.FirstChild( "Mesh" ).Element();
	for ( lpElem; lpElem; lpElem = lpElem->NextSiblingElement("Mesh") )
	{
		std::string lacFile = lacBaseDirectory + lpElem->Attribute("file");
		
		// lOAD THE MESH INTO CAL3d
		int liMeshIndex = mpCoreModel->loadCoreMesh(lacFile);
		assert(liMeshIndex >= 0);
		// store the mesh index
		mMeshIndexes.push_back(liMeshIndex);
	}
	return true;
}
void cSkeletalCoreModel::Deinit()
{
	if ( mpCoreModel)
	{
		delete mpCoreModel;
		mpCoreModel = NULL;
	}
}
void cSkeletalCoreModel::CreateInstance(cSkeletalMesh *lpMesh)
{
	// Create the CalModel instance
	assert(lpMesh);
	lpMesh->mpCal3DModel = new CalModel(mpCoreModel);
	lpMesh->mpCoreModel = this;
	// Attach the meshes in the instance
	std::list<int>::iterator it = mMeshIndexes.begin();
	while (it != mMeshIndexes.end() )
	{
		lpMesh->mpCal3DModel->attachMesh( *it );
		++it;
	}
}

sAnimationDef *cSkeletalCoreModel::GetAnimationDef(const std::string lacAnim)
{
	// look for the animation
	std::list<sAnimationDef>::iterator it = mAnimationDefs.begin();
	while (it != mAnimationDefs.end() )
	{
		std::string lacName = it->macName;
		if ( lacName == lacAnim )
		{
			return &(*it);
		}
		++it;
	}
	return NULL;
}
	