
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CPlayer.generated.h"

UCLASS()
class TPS_API ACPlayer : public ACharacter
{
	GENERATED_BODY()


		//�÷��̾ ���ٵ� �� Ÿ�� ����
private:
	UPROPERTY(EditDefaultsOnly, Category = "Guns")
		TSubclassOf<class ACRifle> RifleClass;

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
private:
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
		float ViewInterpSpeed = 25;

	//���Ʒ� ���� ����
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
		FVector2D PitchRange = FVector2D(-75, +75);

	//�¿� ���� ����
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

	void OnAim();
	void OffAim();

	void OnFire();

	//���ӿ����� 2D��� �ΰ��� (AimYaw ����) ���
private:
	float AimYaw;
	float AimPitch;

	//�÷��̾ ���ٴ� ������ ������(���� Rifle)
private:
	class ACRifle* Rifle;
};
