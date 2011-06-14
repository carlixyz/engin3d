#ifndef EFFECT_H
#define EFFECT_H

#include <string>
#include "../GLHeaders.h"
#include "../../Mathlib/Mathlib.h"
#include "../../Utility/Resource.h"
#include "../../Utility/ResourceHandle.h"


class cEffect : public cResource
{
	public:
		cEffect() { mbLoaded = false; }

		virtual bool Init( const std::string &lacNameId, const std::string &lacFile );
		virtual void Deinit();
		virtual bool IsLoaded() { return mbLoaded; }
		void Reload();

		bool SetTechnique( const std::string &lacTechnique );
		bool SetFirstPass();
		bool SetNextPass();

		void SetParam( const std::string &lacName, const cMatrix &lMatrix );
		void SetParam( const std::string &lacName, float lParam );
		void SetParam( const std::string &lacName, const cVec3 &lParam );
		void SetParam( const std::string &lacName, const cVec4 &lParam );
		void SetParam( const std::string &lacName, cResourceHandle lParam );
		void SetParam(const std::string &lacName, const float * lfParam, int liCount ); // originally: unsigned liCount

	private:
		std::string macFile;
		std::string macLastTechnique ;
		CGeffect mEffect;
		CGtechnique mTechnique;
		CGpass mCurrentPass;

		bool mbLoaded;
};
#endif
