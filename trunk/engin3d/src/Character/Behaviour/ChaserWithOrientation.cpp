#include "ChaserWithOrientation.h"
#include "BehaviourBase.h"

void cChaserWithOrientation::Init(cCharacter *lpCharacter)
{  
	// Inicializar los atributos del comportamiento 
//	mTarget = cVec3( -5.0f, 0.0f, -5.0f); // Not needed for now
	assert(lpCharacter);
	mpCharacter = lpCharacter;	
  mTarget = mpCharacter->GetPosition();
}
void cChaserWithOrientation::SetTarget(float posX, float posY, float posZ)
{
	mTarget = cVec3( posX, posY, posZ );
}

void cChaserWithOrientation::Deinit()
{ 
	// Deinicializar los atributos del comportamiento 
	mpCharacter = NULL;
}

void cChaserWithOrientation::Update(float lfTimestep)
{
  mbEndReached = false;
	//Calcular el vector distancia (diferencia entre la posición del
	// perseguidor y la posición del objetivo a perseguir )
	cVec3 lDistanceVec =  mTarget - mpCharacter->GetPosition(); //CharacterPosicion

  if (lDistanceVec.Length() < 0.2f ) {mbEndReached=true; return;}
	// Calcular la distancia que se movera el perseguidor teniendo en cuenta su velocidad máxima
	float lfDisplacement = mpCharacter->GetSpeed() * lfTimestep;

	// Si la distancia que el perseguidor se va a desplazar es mayor o igual
	// a la distancia que hay a su objetivo, se mueve solo hasta el punto objetivo
	// ( para no pasarse ). Si no, se mueve la distancia establecida
	// (para irse acercando poco a poco al objetivo )
/*	if ( lfDisplacement >= lDistanceVec.Length() )
	{
	//	const cVec3 & lVecPos = mTarget;
		mpCharacter->SetPosition( mTarget);
	}
	else
	{
	// La nueva posición del perseguidor será igual a su posición inicial 
	// más el desplazamiento que debe hacer en la dirección indicada por
	// vector distancia (para moverse así en dirección a su objetivo)

		cVec3 lVecPos = mpCharacter->GetPosition() + ( lfDisplacement * lDistanceVec.Normalize() );
		mpCharacter->SetPosition(lVecPos);
    }
*/
	// SMOOTH ORIENTATION
	if ( lDistanceVec.Length() < 0.1f ) // Poca probabilidad de alcanzár 0.0f
	{
		mpCharacter->SetPosition( mTarget ) ;
		return;
	}

	float lfCosAngle = Dot( mpCharacter->GetFront().Normalize(), lDistanceVec.Normalize() );// Producto escalar

	if ( lfCosAngle > 1.0f )
	{
		lfCosAngle = 1.0f;
	}
	else if (lfCosAngle < -1.0f )
	{
		lfCosAngle = -1.0f;
	}

	float lfAngle = acosf( lfCosAngle );

	float lfAngleDisplacement = mpCharacter->GetAngSpeed() * lfTimestep;

	if (lfAngle > lfAngleDisplacement ) // Si el giro que puede realizar en este frame
		lfAngle = lfAngleDisplacement; // es menor al necesario restringimos la rotación 

	cPlane lPlane;
	// We made a test plane to check if target is at the right or left of Character
	lPlane.Init( mpCharacter->GetRight(), mpCharacter->GetPosition() );

	if ( lPlane.PointTest(mTarget) < 0.0f )
	{
		mpCharacter->SetYaw( mpCharacter->GetYaw() + lfAngle );
	}
	else
	{
		mpCharacter->SetYaw( mpCharacter->GetYaw() - lfAngle );
	}
	
	const cVec3 lVecPos  = mpCharacter->GetPosition() + ( lfDisplacement * mpCharacter->GetFront());
	mpCharacter->SetPosition(lVecPos); // Corregimos la trayectoria modificada por el giro
 
}

//Check if the end point is reached
bool cChaserWithOrientation::EndPointReached()
{
  return mbEndReached;
}



