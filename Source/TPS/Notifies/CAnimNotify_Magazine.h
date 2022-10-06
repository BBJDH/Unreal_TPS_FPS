// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_Magazine.generated.h"

/**
 * 
 */

UENUM()
enum class EMagazineActionType : uint8
{
	Eject, Spawn, End,
};


UCLASS()
class TPS_API UCAnimNotify_Magazine : public UAnimNotify
{
	GENERATED_BODY()

private:
		UPROPERTY(EditAnywhere, Category = "Magazine")
		EMagazineActionType ActionType;
public:
	FString GetNotifyName_Implementation() const override;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

};
