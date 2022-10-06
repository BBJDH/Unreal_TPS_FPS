
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CWeaponComponent.generated.h"


UENUM(BlueprintType)
enum class EWeaponType: uint8
{
	AR4,AK47,Pistol, Max,
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponTypeChanged, EWeaponType, InPrevType, EWeaponType, InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPS_API UCWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Weapon")
		TArray<TSubclassOf<class ACWeapon>> WeaponClasses;

	UPROPERTY(EditAnywhere, Category = "Weapon")
		TSubclassOf<class UCUserWidget_HUD> HUDClass;


public:
	FORCEINLINE bool IsUnarmedMode() { return Type == EWeaponType::Max; }
	FORCEINLINE bool IsAR4Mode() { return Type == EWeaponType::AR4; }
	FORCEINLINE bool IsAK47Mode() { return Type == EWeaponType::AK47; }
	FORCEINLINE bool IsPistolMode() { return Type == EWeaponType::Pistol; }

public:	
	UCWeaponComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:
	//현재 세팅된 무기 반환
	class ACWeapon* GetCurrWeapon();

public:
	void SetUnarmedMode();
	void SetAR4Mode();
	void SetAK47Mode();
	void SetPistolMode();


private:
	void SetMode(EWeaponType InType);
	void ChangeType(EWeaponType InType);


public:
	bool IsInAim();
	bool IsFiring();

	FVector GetLeftHandLocation();

public:
	void Begin_Equip();
	void End_Equip();

	void Begin_Aim();
	void End_Aim();

	void Begin_Fire();
	void End_Fire();
	void ToggleAutoFire();

	void Reload();
	void Eject_Magazine();
	void Spawn_Magazine();
	void Load_Magazine();
	void End_Reload();

public:
	//델리게이트
	FWeaponTypeChanged OnWeaponTypeChange;

public:
	//무기 생성 목록
	TArray<class ACWeapon*> Weapons;

private:
	class ACPlayer* Owner;
	class UCUserWidget_HUD* HUD;

private:
	//무기타입
	EWeaponType Type = EWeaponType::Max;
		
};
