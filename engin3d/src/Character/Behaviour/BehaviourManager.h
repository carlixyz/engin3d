#ifndef CBEHAVIOURMANAGER_H
#define	CBEHAVIOURMANAGER_H

#include <list>
#include "../../Utility/Singleton.h"
#include "ChaserNoOrientation.h"
#include "ChaserSnapOrientation.h"
#include "ChaserWithOrientation.h"
//#include "BehaviourBase.h" // Not needed included before

	enum eBehaviours{
		eCHASER_NO_ORIENTATION = 0,
		eCHASER_SNAP_ORIENTATION,
		eCHASER_WITH_ORIENTATION,

		eBEHAVIOURS_Count
	};

class cBehaviourManager :  public cSingleton<cBehaviourManager>
{
 private:  

	friend class cSingleton<cBehaviourManager>;
  
	typedef std::list<cBehaviourBase *> cBehaviourList;
	typedef cBehaviourList::iterator cBehaviourListIt;
	cBehaviourList mBehaviours; // Lista de Comportamients !

 public:
	// return true
	  bool Init();

	// delete all Behaviour´s of the list
	  bool Deinit();

	// Create a new behaviour of kind choosen in leBehaviour and
	// inserts it in the list, returning a pointer of the behaviour created
	  cBehaviourBase* CreateBehaviour( eBehaviours leBehaviour );

	// Delete all list´s behaviours
	  void DestroyBehaviour( cBehaviourBase* lpBehaviour);	

 protected:
	   cBehaviourManager() {;}
};

#endif	/* CBEHAVIOURMANAGER_H */
