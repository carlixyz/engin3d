#ifndef CCHARMANAGER_H
#define	CCHARMANAGER_H

#include <list>
#include "../Utility/Singleton.h"

#include "Character.h"

class cCharacterManager :  public cSingleton<cCharacterManager>
{
 private:  

	friend class cSingleton<cCharacterManager>;
	friend class cCharacter;
  
	typedef std::list<cCharacter *> cCharacterList;
	typedef cCharacterList::iterator cCharacterListIt;
	cCharacterList mCharacters; // Lista de Personajes !

 public:
	  // return true
	  bool Init();

	  // Call all Character´s Deinit
	  bool Deinit();

	  // Call all Character´s update
	  void Update(float lfTimeStep);

	  // Call all Character´s Render
	  void Render();

	  // Create a new Char, call his  init() and save it in the list
	  // Return a pointer of the created char
	  cCharacter* CreateCharacter();

	  // Delete character from the list and call his Deinit()
	  void DestroyCharacter( cCharacter* lpCharacter);
	

 protected:
	   cCharacterManager() {;}
};

#endif	/* CCHARMANAGER_H */
