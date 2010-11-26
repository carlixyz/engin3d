#ifndef CBEHAVIOURBASE_H
#define	CBEHAVIOURBASE_H

class cCharacter; // One Foward Declaration for a lot of crossed includes (What a mess!)

class cBehaviourBase // Abstraction of Class
{
public:
	// initialize behaviours attribs and it�s called
	// in the moment that�s asociated this behaviour with a character
	virtual void Init(cCharacter * lpCharacter) = 0; // virtual pure foo 

	// Deinitialize the behaviour�s attribs and it�s called
	// in the moment when the Character is deleted
	virtual void Deinit() = 0; // virtual pure foo 

	// This foo it�s called in every frame for update the logic
	// asociated with the current behaviour
	virtual void Update(float lfTimestep) = 0; // virtual pure foo 

	// This foo it�s for set the target position from anywhere
	//virtual void SetTarget( const cVec3 &lvTarget ) = 0; // It Doesn�t Work ! 
};

#endif /* CBEHAVIOURBASE_H */

