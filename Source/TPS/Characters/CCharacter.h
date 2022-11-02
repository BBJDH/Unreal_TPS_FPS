
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CCharacter.generated.h"

UCLASS(Abstract)
class TPS_API ACCharacter : public ACharacter
{
	GENERATED_BODY()
	
	

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
		float ViewInterpSpeed = 25;

	//위아래 각도 제한
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
		FVector2D PitchRange = FVector2D(-75, +40);

	//좌우 각도 제한
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
		FVector2D YawRange = FVector2D(-90, +90);

protected:
	UPROPERTY(VisibleDefaultsOnly)
	class UCWeaponComponent* Weapon;



public:
	FORCEINLINE float GetAimPitch() { return AimPitch; }
	FORCEINLINE float GetAimYaw() { return AimYaw; }

	//에임오프셋 2D라면 두가지 (AimYaw 까지) 사용

public:
	ACCharacter();
	void Tick(float DeltaTime);


protected:
	virtual void BeginPlay() override;

private:
	float AimYaw;
	float AimPitch;

};
