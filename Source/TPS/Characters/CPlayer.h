
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CPlayer.generated.h"

UCLASS()
class TPS_API ACPlayer : public ACharacter
{
	GENERATED_BODY()


		//에임 오프셋에 사용
	/*
	어떤 애니메이션을 쓰던지 끊기는 상황을 대비하여
	게임 로직 쓰레드, 렌더 쓰레드
	언리얼은 전부 쓰레드 기반
	Game Loop => Tick =>시스템 퍼포먼스에 따라 다름
	Render => 1/60

	보간은 Tick에서 처리되므로 Render 쓰레드와 타이밍이 같지 않다
	때문에 DeltaTIme을 이용하여 보간 처리
	Tick에서 렌더링을 제어한다면 꼭 DeltaTIme을 사용
	*/


	//플레이어가 들고다딜 총 타입 지정
//private:
//	UPROPERTY(EditDefaultsOnly, Category = "Guns")
//		TSubclassOf<class ACWeapon> RifleClass;
private:
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
		float ViewInterpSpeed = 25;

	//위아래 각도 제한
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
		FVector2D PitchRange = FVector2D(-75, +75);

	//좌우 각도 제한
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
		FVector2D YawRange = FVector2D(-90, +90);

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
		float LimitPitchAngle = 40;	//최대 누적 반동각

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly)
		class UCameraComponent* Camera;

	UPROPERTY(VisibleDefaultsOnly)
		class UCWeaponComponent* Weapon;

public:

	//FORCEINLINE class ACRifle* GetRifle() { return Rifle; }

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


public:
	void UseControlRotation();
	void NotUseControlRotation();

	//에임오프셋 2D라면 두가지 (AimYaw 까지) 사용
private:
	float AimYaw;
	float AimPitch;

	//플레이어가 들고다닐 라이플 포인터(하위 Rifle)
private:
	//class ACRifle* Rifle;
};
