#include "ChaserSnapOrientation.h"


void cChaserSnapOrientation::Init(cCharacter *lpCharacter)
{ // Inicializar los atributos del comportamiento 
//	mTarget = cVec3( -5.0f, 0.0f, -5.0f); // Not needed for now
	mpCharacter = lpCharacter;	
}

void cChaserSnapOrientation::Deinit()
{ // Deinicializar los atributos del comportamiento 
	mpCharacter = NULL;
}

void cChaserSnapOrientation::SetTarget(float posX, float posY, float posZ)
{
	mTarget = cVec3( posX, posY, posZ );
}

void cChaserSnapOrientation::Update(float lfTimestep)
{
	//Calcular el vector distancia (diferencia entre la posición del
	// perseguidor y la posición del objetivo a perseguir )
	cVec3 lDistanceVec =  mTarget - mpCharacter->GetPosition(); //CharacterPosicion

	// Calcular la distancia que se movera el perseguidor teniendo en cuenta su velocidad máxima
	float lfDisplacement = mpCharacter->GetSpeed() * lfTimestep;

	// Si la distancia que el perseguidor se va a desplazar es mayor o igual
	// a la distancia que hay a su objetivo, se mueve solo hasta el punto objetivo
	// ( para no pasarse ). Si no, se mueve la distancia establecida
	// (para irse acercando poco a poco al objetivo )
	if ( lfDisplacement >= lDistanceVec.Length() )
	{
//		const cVec3 & lVecPos = mTarget;
		mpCharacter->SetPosition( mTarget);
    mbEndReached = true;
	}
	else
	{
	// La nueva posición del perseguidor será igual a su posición inicial 
	// más el desplazamiento que debe hacer en la dirección indicada por
	// vector distancia (para moverse así en dirección a su objetivo)

		cVec3 lVecPos = mpCharacter->GetPosition() + ( lfDisplacement * lDistanceVec.Normalize() );
		mpCharacter->SetPosition(lVecPos);
    mbEndReached = false;
	}
	if ( lDistanceVec.Length() > 0.001f ) // Orientación Inmediata del perseguidor
		mpCharacter->SetYaw( atan2f(lDistanceVec.x, lDistanceVec.z	) );

}

//Check if the end point is reached
bool cChaserSnapOrientation::EndPointReached()
{
  return mbEndReached;
}


