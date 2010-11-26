#include "ChaserWithOrientation.h"
#include "BehaviourBase.h"

void cChaserWithOrientation::Init(cCharacter *lpCharacter)
{  
	// Inicializar los atributos del comportamiento 
	mTarget = cVec3( -5.0f, 0.0f, -5.0f);
	mpCharacter = lpCharacter;	
}

void cChaserWithOrientation::Deinit()
{ 
	// Deinicializar los atributos del comportamiento 
	mpCharacter = NULL;
}

void cChaserWithOrientation::Update(float lfTimestep)
{

	//Calcular el vector distancia (diferencia entre la posici�n del
	// perseguidor y la posici�n del objetivo a perseguir )
	cVec3 lDistanceVec =  mTarget - mpCharacter->GetPosition(); //CharacterPosicion

	// Calcular la distancia que se movera el perseguidor teniendo en cuenta su velocidad m�xima
	float lfDisplacement = mpCharacter->GetSpeed() * lfTimestep;

	// Si la distancia que el perseguidor se va a desplazar es mayor o igual
	// a la distancia que hay a su objetivo, se mueve solo hasta el punto objetivo
	// ( para no pasarse ). Si no, se mueve la distancia establecida
	// (para irse acercando poco a poco al objetivo )
	if ( lfDisplacement >= lDistanceVec.Length() )
	{
//		const cVec3 & lVecPos = mTarget;
		mpCharacter->SetPosition( mTarget);
	}
	else
	{
	// La nueva posici�n del perseguidor ser� igual a su posici�n inicial 
	// m�s el desplazamiento que debe hacer en la direcci�n indicada por
	// vector distancia (para moverse as� en direcci�n a su objetivo)

		cVec3 lVecPos = mpCharacter->GetPosition() + ( lfDisplacement * lDistanceVec.Normalize() );
		mpCharacter->SetPosition(lVecPos);
	}
	// SMOOTH ORIENTATION
	if ( lDistanceVec.Length() < 0.1f ) // Poca probabilidad de alcanz�r 0.0f
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
		lfAngle = lfAngleDisplacement; // es menor al necesario restringimos la rotaci�n 

	cPlane lPlane; // We are going to make a test plane
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

