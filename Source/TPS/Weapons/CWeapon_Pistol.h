
#pragma once

#include "CoreMinimal.h"
#include "Weapons/CWeapon.h"
#include "CWeapon_Pistol.generated.h"

/**
 * 
 */
UCLASS()
class TPS_API ACWeapon_Pistol : public ACWeapon
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Equip")
		FName LeftHandSocketName;
protected:
	virtual void BeginPlay() override;

public:
	ACWeapon_Pistol();

public:
	void Begin_Equip() override;

	void Begin_Aim() override;
	void End_Aim() override;
	
};
