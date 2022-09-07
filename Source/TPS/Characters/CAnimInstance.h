#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TPS_API UCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	

       //Plyaer 의 값 받아올 곳
protected:
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Character")
        float Speed;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Character")
        float Direction;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Character")
        float AimPitch;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Character")
        float AimYaw;


public:
    virtual void NativeBeginPlay() override;
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
    class ACPlayer* Owner;
};
