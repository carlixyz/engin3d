#include "ChaserNoOrientation.h"


void cChaserNoOrientation::Init(cCharacter *lpCharacter)
{ 
	// Inicializar los atributos del comportamiento 
//	mTarget = cVec3( -5.0f, 0.0f, -5.0f); // Not needed for now
	mpCharacter = lpCharacter;	
  mbEndReached = false;
}

void cChaserNoOrientation::Deinit()
{ 
	mpCharacter = NULL;
}
void cChaserNoOrientation::SetTarget(float posX, float posY, float posZ)
{
	mTarget = cVec3( posX, posY, posZ );
}
void cChaserNoOrientation::Update(float lfTimestep)
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
    mbEndReached = true;
	}
	else
	{
	// La nueva posici�n del perseguidor ser� igual a su posici�n inicial 
	// m�s el desplazamiento que debe hacer en la direcci�n indicada por
	// vector distancia (para moverse as� en direcci�n a su objetivo)

		cVec3 lVecPos = mpCharacter->GetPosition() + ( lfDisplacement * lDistanceVec.Normalize() );
		mpCharacter->SetPosition(lVecPos);
    mbEndReached = false;
	}
}

//Check if the end point is reached
bool cChaserNoOrientation::EndPointReached()
{
  return mbEndReached;
}


