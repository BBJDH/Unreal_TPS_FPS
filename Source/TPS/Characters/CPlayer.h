
#pragma once

#include "CoreMinimal.h"
#include "Characters/CCharacter.h"
#include "CPlayer.generated.h"




UCLASS()
class TPS_API ACPlayer : public ACCharacter
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



private:

	// UPROPERTY(EditDefaultsOnly, Category = "Camera")
	// 	float LimitPitchAngle = 40;	//�ִ� ���� �ݵ���

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly)
		class UCameraComponent* Camera;

	//1��Ī
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



	//�÷��̾ ���ٴ� ������ ������(���� Rifle)

};
