
#pragma once

#include "CoreMinimal.h"
#include "Characters/CCharacter.h"
#include "CPlayer.generated.h"




UCLASS()
class TPS_API ACPlayer : public ACCharacter
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



private:

	// UPROPERTY(EditDefaultsOnly, Category = "Camera")
	// 	float LimitPitchAngle = 40;	//최대 누적 반동각

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly)
		class UCameraComponent* Camera;

	//1인칭
	UPROPERTY(VisibleDefaultsOnly)
		class USkeletalMeshComponent* Arms;

	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* Backpack;



public:

	FORCEINLINE class USkeletalMeshComponent * GetArms() { return Arms; }
	FORCEINLINE class UStaticMeshComponent * GetBackpack() { return Backpack; }

public:
	ACPlayer();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void OnMoveForward(float AxisValue);
	void OnMoveRight(float AxisValue);
	void OnVerticalLook(float AxisValue);
	void OnHorizontalLook(float AxisValue);


public:
	void UseControlRotation();
	void NotUseControlRotation();



	//플레이어가 들고다닐 라이플 포인터(하위 Rifle)

};
