#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Weapons/CWeaponComponent.h"
#include "CAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TPS_API UCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	

       //Plyaer �� �� �޾ƿ� ��
protected:
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Character")
        float Speed;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Character")
        float Direction;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Character")
        float AimPitch;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Character")
        float AimYaw;

    //���� IK �� ����
protected:
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Guns")
        EWeaponType WeaponType = EWeaponType::Max;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Guns")
        FVector LeftHandLocation = FVector(-35, 15.5f, 5);
    
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Guns")
        bool bInAim;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Guns")
        bool bFiring;


    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Guns")
        bool bUseIK;
public:
    virtual void NativeBeginPlay() override;
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:

    void SetPlayerState();
    void SetWeaponState();

private:
    UFUNCTION()
        void OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType);
private:
    class ACPlayer* Owner;
    class UCWeaponComponent* Weapon;
};
