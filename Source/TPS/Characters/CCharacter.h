
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CCharacter.generated.h"

UCLASS(Abstract)
class TPS_API ACCharacter : public ACharacter
{
	GENERATED_BODY()
	
	
protected:
	UPROPERTY(VisibleDefaultsOnly)
	class UCWeaponComponent* Weapon;


public:
	ACCharacter();


protected:
	virtual void BeginPlay() override;

};
