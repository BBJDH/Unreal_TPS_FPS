
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

	//���Ʒ� ���� ����
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
		FVector2D PitchRange = FVector2D(-75, +40);

	//�¿� ���� ����
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
		FVector2D YawRange = FVector2D(-90, +90);

protected:
	UPROPERTY(VisibleDefaultsOnly)
	class UCWeaponComponent* Weapon;



public:
	FORCEINLINE float GetAimPitch() { return AimPitch; }
	FORCEINLINE float GetAimYaw() { return AimYaw; }

	//���ӿ����� 2D��� �ΰ��� (AimYaw ����) ���

public:
	ACCharacter();
	void Tick(float DeltaTime);


protected:
	virtual void BeginPlay() override;

private:
	float AimYaw;
	float AimPitch;

};
