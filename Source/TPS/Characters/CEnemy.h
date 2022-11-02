
#pragma once

#include "CoreMinimal.h"
#include "Characters/CCharacter.h"
#include "CEnemy.generated.h"


UCLASS()
class TPS_API ACEnemy : public ACCharacter
{
	GENERATED_BODY()
public:
	ACEnemy();

protected:
	virtual void BeginPlay() override;	
};
