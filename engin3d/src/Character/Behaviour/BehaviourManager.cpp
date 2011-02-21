#include "BehaviourManager.h"

#include "ChaserNoOrientation.h"
#include "ChaserSnapOrientation.h"
#include "ChaserWithOrientation.h"
#include "Patrol.h"
#include "PlayerController.h"

bool cBehaviourManager::Init()
{
	return true;
}

bool cBehaviourManager::Deinit()
{
	for (cBehaviourListIt lpBehaviourIt = mBehaviours.begin(); lpBehaviourIt !=  mBehaviours.end() ; ++lpBehaviourIt )
	{
		//WE DON큈 Call all Behaviour큦 Deinit (*lpBehaviourIt)->Deinit();		
		delete * lpBehaviourIt;// Free his space in memory (:D  
	} 
	mBehaviours.clear();// cleaning the wild pointers in the list

	return true;
}
cBehaviourBase* cBehaviourManager::CreateBehaviour( eBehaviours leBehaviour)
{
	
	switch(leBehaviour)
	{
	case eCHASER_NO_ORIENTATION:
		{
			cBehaviourBase* lpBehaviour = new cChaserNoOrientation();
			assert(lpBehaviour);
			mBehaviours.push_back( lpBehaviour );
			return lpBehaviour;
			break;
		}
	case eCHASER_SNAP_ORIENTATION:
		{
			cBehaviourBase* lpBehaviour = new cChaserSnapOrientation();
			assert(lpBehaviour);
			mBehaviours.push_back( lpBehaviour );
			return lpBehaviour;
			break;
		}
	case eCHASER_WITH_ORIENTATION:
		{
			cBehaviourBase* lpBehaviour = new cChaserWithOrientation();
			assert(lpBehaviour);
			mBehaviours.push_back( lpBehaviour );
			return lpBehaviour;
			break;
		}
  case ePATROL:
    {
      cBehaviourBase* lpBehaviour = new cPatrol();
	  assert(lpBehaviour);
      mBehaviours.push_back( lpBehaviour );
      return lpBehaviour;
      break;
    }
  case ePLAYER_CONTROLLER:
    {
      cBehaviourBase* lpBehaviour = new cPlayerController();
	  assert(lpBehaviour);
      mBehaviours.push_back( lpBehaviour );
      return lpBehaviour;
      break;
    }
	default:
		return NULL; //WE DON큈 Call all Behaviour큦 Init
	}
}


void cBehaviourManager::DestroyBehaviour(cBehaviourBase *lpBehaviour)
{
		// Recorremos la Lista de objetos...
	cBehaviourListIt lpBehaviourIt = mBehaviours.begin();
	assert(lpBehaviour);
	while ( lpBehaviourIt != mBehaviours.end()  )
	{		
		 if( (*lpBehaviourIt) == lpBehaviour ) // si encontramos el mismo objeto ...
		 {
		//	 (*lpBehaviourIt)->Deinit(); // NO Llamamos a su deinit

			 delete (* lpBehaviourIt); //Borramos el OBJETO
			 
			 // y luego lo eliminamos de la lista
			 lpBehaviourIt = mBehaviours.erase(lpBehaviourIt); // Borrar el PUNTERO al objeto
			 break;
		 }
		 else // sino seguimos buscando ...
		 {
			 lpBehaviourIt++;
		 }
	}
}