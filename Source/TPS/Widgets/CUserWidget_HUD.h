// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Weapons/CWeaponComponent.h"
#include "CUserWidget_HUD.generated.h"

/**
 * 
 */
UCLASS()
class TPS_API UCUserWidget_HUD : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent)
		void UpdateWeaponName(EWeaponType InType);

	UFUNCTION(BlueprintImplementableEvent)
		void UpdateMagazine(uint8 InCount, uint8 IndMax);

	UFUNCTION(BlueprintImplementableEvent)
		void OnAutoFire();

	UFUNCTION(BlueprintImplementableEvent)
		void OffAutoFire();
};
