#include "Character.h"

#include "../Graphics/GraphicManager.h"
#include "../Lua/LuaManager.h"

void cCharacter::Init(  int liId )
{	
	miId = liId;
	mPosition = cVec3( 0.0f, 0.0f, 0.0f) ;
  mColour = cVec3 (1.0f, 0.0f, 0.0f);
	mfYaw = 0.0f;
	mfSpeed = 0.0f;
	mfAngSpeed = 0.0f;

	mpActiveBehaviour = NULL;
}

void cCharacter::Deinit()
{
	if (mpActiveBehaviour)
	{
		mpActiveBehaviour->Deinit() ;
		cBehaviourManager::Get().DestroyBehaviour( mpActiveBehaviour );
		//this is it
	}
}

void cCharacter::Update(float lfTimeStep)
{
	if (mpActiveBehaviour)	mpActiveBehaviour->Update(lfTimeStep);
}

void cCharacter::Render()
{
	// Compose the world matrix
	cMatrix lWorld;
	lWorld.LoadIdentity();
  cGraphicManager::Get().SetWorldMatrix(lWorld);
  //Draw each path 
  cLuaManager::Get().CallLua("DrawPath", this->GetId(), (int *)NULL);  
  
  //Draw the player
  lWorld.LoadIdentity();
	lWorld.LoadRotation(cVec3(0.0f, 1.0f, 0.0f), mfYaw);
	lWorld.SetPosition( mPosition );

	// Set the World matrix
	cGraphicManager::Get().SetWorldMatrix(lWorld);

	// Draw character ( a dot for position & a line for orientation)
	// char default orientation it´s looking at +Z axis
	cGraphicManager::Get().DrawPoint( cVec3(0.0f, 0.0f, 0.0f),
		mColour );

	cGraphicManager::Get().DrawLine( cVec3(0.0f, 0.0f, 0.0f),
		cVec3( 0.0f, 0.0f, 1.0f), mColour );


}

void cCharacter::SetColour(const cVec3 &lColour)
{
  mColour = lColour;
}

void cCharacter::SetPosition(const cVec3 &lPosition)
{
	mPosition = lPosition;
}

void cCharacter::SetYaw(float lfYaw)
{
	mfYaw = lfYaw;
}

void cCharacter::SetSpeed(float lfSpeed)
{
	mfSpeed = lfSpeed;
}

void cCharacter::SetAngSpeed(float lfAngSpeed)
{
	mfAngSpeed = lfAngSpeed;
}

void cCharacter::SetActiveBehaviour(cBehaviourBase *lpBehaviour)
{	
	if ( mpActiveBehaviour )	mpActiveBehaviour->Deinit();
	
	 mpActiveBehaviour = lpBehaviour;

	if ( mpActiveBehaviour )	mpActiveBehaviour->Init(this);
	
}
void cCharacter::SetActiveBehaviour( cBehaviourBase * lpBehaviour, float posX, float posY, float posZ )
{
		mpActiveBehaviour = lpBehaviour;
		mpActiveBehaviour->Init(this);
		mpActiveBehaviour->SetTarget(  posX, posY, posZ );
}


