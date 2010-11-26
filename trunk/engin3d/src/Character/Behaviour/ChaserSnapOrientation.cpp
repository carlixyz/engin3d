#include "ChaserSnapOrientation.h"


void cChaserSnapOrientation::Init(cCharacter *lpCharacter)
{ // Inicializar los atributos del comportamiento 
	mTarget = cVec3( 5.0f, 0.0f, 3.0f);
	mpCharacter = lpCharacter;	
}

void cChaserSnapOrientation::Deinit()
{ // Deinicializar los atributos del comportamiento � cuales ?
	mpCharacter = NULL;
}

void cChaserSnapOrientation::Update(float lfTimestep)
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
	if ( lDistanceVec.Length() > 0.001f ) // Orientaci�n Inmediata del perseguidor
		mpCharacter->SetYaw( atan2f(lDistanceVec.x, lDistanceVec.z	) );

}
