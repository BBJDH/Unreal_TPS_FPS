
#pragma once

#include "CoreMinimal.h"
#include "Weapons/CWeapon.h"
#include "CWeapon_AK47.generated.h"

UCLASS()
class TPS_API ACWeapon_AK47 : public ACWeapon
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Equip")
		FName LeftHandSocketName;

	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* Dotsight;
public:
	ACWeapon_AK47();

public:
	void Begin_Equip() override;
	void End_Equip() override;

	void Begin_Aim() override;
	void End_Aim() override;

};
