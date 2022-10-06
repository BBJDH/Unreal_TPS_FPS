
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CPlayer.generated.h"

UCLASS()
class TPS_API ACPlayer : public ACharacter
{
	GENERATED_BODY()


		//���� �����¿� ���
	/*
	� �ִϸ��̼��� ������ ����� ��Ȳ�� ����Ͽ�
	���� ���� ������, ���� ������
	�𸮾��� ���� ������ ���
	Game Loop => Tick =>�ý��� �����ս��� ���� �ٸ�
	Render => 1/60

	������ Tick���� ó���ǹǷ� Render ������� Ÿ�̹��� ���� �ʴ�
	������ DeltaTIme�� �̿��Ͽ� ���� ó��
	Tick���� �������� �����Ѵٸ� �� DeltaTIme�� ���
	*/


	//�÷��̾ ���ٵ� �� Ÿ�� ����
//private:
//	UPROPERTY(EditDefaultsOnly, Category = "Guns")
//		TSubclassOf<class ACWeapon> RifleClass;
private:
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
		float ViewInterpSpeed = 25;

	//���Ʒ� ���� ����
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
		FVector2D PitchRange = FVector2D(-75, +75);

	//�¿� ���� ����
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
		FVector2D YawRange = FVector2D(-90, +90);

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
		float LimitPitchAngle = 40;	//�ִ� ���� �ݵ���

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

	//���ӿ����� 2D��� �ΰ��� (AimYaw ����) ���
private:
	float AimYaw;
	float AimPitch;

	//�÷��̾ ���ٴ� ������ ������(���� Rifle)
private:
	//class ACRifle* Rifle;
};
