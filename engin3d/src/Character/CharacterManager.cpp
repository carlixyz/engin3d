
#include "CharacterManager.h"


bool cCharacterManager::Init()
{
return true;
}

bool cCharacterManager::Deinit()
{
		for (cCharacterListIt lpCharIterator = mCharacters.begin(); lpCharIterator !=  mCharacters.end() ; ++lpCharIterator )
	{
		// Call all Character´s Deinit
		(*lpCharIterator)->Deinit();
		
		delete * lpCharIterator;// Free his space in memory (:D  
	} 
	mCharacters.clear();// cleaning the wild pointers in the list

	return true;
}

void cCharacterManager::Update(float lfTimeStep)
{
		for (cCharacterListIt lpCharIterator = mCharacters.begin(); lpCharIterator !=  mCharacters.end() ; ++lpCharIterator )
	{
		// Call all Character´s update
		(*lpCharIterator)->Update(lfTimeStep);
	} 
}

void cCharacterManager::Render()
{
		for (cCharacterListIt lpCharIterator = mCharacters.begin(); lpCharIterator !=  mCharacters.end() ; ++lpCharIterator )
	{
		// Call all Character´s Render
		(*lpCharIterator)->Render() ;
	} 
}

cCharacter* cCharacterManager::CreateCharacter()
{
	cCharacter* lpCharacter = new cCharacter();
	lpCharacter->Init();
	mCharacters.push_back(lpCharacter);

	return lpCharacter;
}

void cCharacterManager::DestroyCharacter(cCharacter *lpCharacter)
{
	// Recorremos la Lista de objetos...
	cCharacterListIt lpCharIterator = mCharacters.begin();
	while ( lpCharIterator != mCharacters.end()  )
	{		
		 if( (*lpCharIterator) == lpCharacter ) // si encontramos el objeto ...
		 {
			 (*lpCharIterator)->Deinit(); // Llamamos a su deinit

			 delete (* lpCharIterator); //Borramos el OBJETO
			 
			 // y luego lo eliminamos de la lista
			 lpCharIterator = mCharacters.erase(lpCharIterator); // Borrar el PUNTERO al objeto
		 }
		 else // sino seguimos buscando ...
		 {
			 lpCharIterator++;
		 }
	}	

}

