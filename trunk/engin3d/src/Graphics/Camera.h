#ifndef CAMERA_H
#define CAMERA_H

#include "../MathLib/MathLib.h"

class cCamera
{
private:
	cMatrix mView;
	cMatrix mProj;
	cMatrix mViewProj;	// same as View Matrix * Proj Matrix
	inline void RefreshViewProj() { mViewProj = mView * mProj; }

public:
	void Init();
	void SetLookAt( const cVec3 &lvPosition, const cVec3 &lvTarget,// LvUpWorld by default is +Y Vector
		const cVec3 &lvUpWorld = cVec3(0.0f, 1.0f, 0.0f) );	 

	void SetOrtho(	float lfLeft, float lfRight,
					float lfBottom, float lfTop,
					float lfZNear, float lfZNFar ); // Set 2D Proyection

	void SetPerspective( float lfFOVY_Rad, float lfAspect,
						 float lfZNear, float lfZFar );// 3D Proyection

	cVec3 GetFront()	const { mView.GetFront(); }
	cVec3 GetLeft()		const { mView.GetLeft();  }
	cVec3 GetRight()	const { mView.GetRight(); }
	cVec3 GetUp()		const { mView.GetUp();	  }
	cVec3 GetPosition() const { mView.GetPosition(); }
	
	const cMatrix &GetView() const { return mView; }
	const cMatrix &GetProj() const { return mProj; }
	const cMatrix &GetViewProj() const { return mViewProj; }
};
// GL_MODELVIEW = WorldMatrix * ViewMatrix
// GL_PROJECTION = ProyectionMatrix

#endif