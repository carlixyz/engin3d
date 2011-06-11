#ifndef SKELETAL_MESH_H
#define SKELETAL_MESH_H

#include <string>
#include "SkeletalCoreModel.h"
#include "../Meshes/Mesh.h"

class CalModel;

class cSkeletalMesh : public cMesh
{
	public:
		cSkeletalMesh() : cMesh() { }
		friend class cSkeletalCoreModel;
		virtual bool Init( const std::string &lacNameID,
							void * lpMemoryData,
							int liDataType) ;
		virtual void Deinit();
		virtual void Update(float lfTimeStep);
		virtual void RenderMesh();
		virtual bool IsLoaded() { return (mpCal3DModel != NULL); }

		void PrepareRender(cResourceHandle lMaterial);		
		void RenderSkeleton();
		bool PlayAnim(const std::string &lacAnimName,
						float lfWeight, float lfDelayIn, float lfDelayOut = 0.0f );
		void StopAnim(const std::string &lacAnimName, float lfDelayOut = 0.0f );
	private:
		CalModel *mpCal3DModel;
		cSkeletalCoreModel * mpCoreModel;
};

#endif
