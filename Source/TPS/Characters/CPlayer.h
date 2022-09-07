
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CPlayer.generated.h"

UCLASS()
class TPS_API ACPlayer : public ACharacter
{
	GENERATED_BODY()


		//에임 오프셋에 사용
private:
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
		float ViewInterpSpeed = 25;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
		FVector2D PitchRange = FVector2D(-75, +75);

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
		FVector2D YawRange = FVector2D(-90, +90);

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly)
		class UCameraComponent* Camera;

public:
	FORCEINLINE float GetAimPitch() { return AimPitch; }
	FORCEINLINE float GetAimYaw() { return AimYaw; }

public:
	ACPlayer();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void OnMoveForward(float AxisValue);
	void OnMoveRight(float AxisValue);
	void OnVerticalLook(float AxisValue);
	void OnHorizontalLook(float AxisValue);


	//에임오프셋 2D라면 두가지 (AimYaw 까지) 사용
private:
	float AimYaw;
	float AimPitch;
};
