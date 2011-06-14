#ifndef CCHARACTER_H
#define	CCHARACTER_H

#include "../Libraries/MathLib/MathLib.h"
#include "Behaviour/BehaviourManager.h"

class cCharacter 
{
 private:  

	int miId;
	cVec3 mPosition;
  cVec3 mColour;
	float mfYaw;
	float mfSpeed;
	float mfAngSpeed;

	cBehaviourBase* mpActiveBehaviour;

 public:
	  // initialize vars
	  void Init( int liId );

	  // empty definition
	  void Deinit();

	  // empty definition
	  void Update(float lfTimeStep);

	  // Renders a point showing character´s position
	  // and a little line starting from such point that
	  // shows the character´s orientation
	  void Render();

	  //Getters
	  inline const int &GetId() const { return miId; }
  	  inline const cVec3 &GetPosition() const { return mPosition; }
  	  inline const float GetYaw() const { return mfYaw; }
  	  inline const float GetSpeed() const { return mfSpeed; }
  	  inline const float GetAngSpeed() const { return mfAngSpeed; }

	  // Return front Vector of Character
  	  inline cVec3 GetFront() const { return cVec3( sinf(mfYaw), 0.0f, cosf(mfYaw) )  ;}

	  // Return Left vector of character
	  inline cVec3 GetLeft() const { return cVec3( sinf(mfYaw + HALF_PI), 0.0f, cosf(mfYaw + HALF_PI) ) ;}

  	  // Return Right vector of character
	  inline cVec3 GetRight() const { return -GetLeft(); }

	  inline cBehaviourBase* GetActiveBehaviour() { return mpActiveBehaviour; }// Get a pointer of the current Behaviour

	  // Setters
	  void SetPosition( const cVec3 &lPosition );
    void SetColour( const cVec3 &lColour );
	  void SetYaw( float lfYaw );
	  void SetSpeed( float lfSpeed );
	  void SetAngSpeed( float lfAngSpeed );

  	  void SetActiveBehaviour( cBehaviourBase * lpBehaviour ); // Set current behaviour
  	  void SetActiveBehaviour( cBehaviourBase * lpBehaviour, float posX, float posY, float posZ ); // Same but with Target
};

#endif	/* CCHARACTER_H */
