
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CWeapon.generated.h"




USTRUCT(BlueprintType)
struct FWeaponAimData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		float TargetArmLength;

	UPROPERTY(EditAnywhere)
		FVector SocketOffset;

	UPROPERTY(EditAnywhere)
		float FieldOfView;

public:
	void SetData(class ACharacter* InOwner);
	void SetDataByNoneCurve(class ACharacter* InOwner);
};



UCLASS()
class TPS_API ACWeapon : public AActor
{
	GENERATED_BODY()
	

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Data")
		FWeaponAimData BaseData;

	UPROPERTY(EditDefaultsOnly, Category = "Data")
		FWeaponAimData AimData;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Equip")
		class UAnimMontage* EquipMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Equip")
		float EquipMontage_PlayRate =1;

	UPROPERTY(EditDefaultsOnly, Category = "Equip")
		FVector LeftHandLocation;


protected:
	UPROPERTY(EditDefaultsOnly, Category = "Aiming")
		class UCurveFloat* AimCurve;

	UPROPERTY(EditDefaultsOnly, Category = "Aiming")
		float AimingSpeed = 200;


protected:
	UPROPERTY(EditDefaultsOnly, Category = "Magazine")
		uint8 MaxMagazineCount = 30;

	UPROPERTY(EditDefaultsOnly, Category = "Magazine")
		class UAnimMontage* ReloadMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Magazine")
		FName MagazineBoneName;

	UPROPERTY(EditDefaultsOnly, Category = "Magazine")
		TSubclassOf<class ACMagazine> MagazineClass;

	UPROPERTY(EditDefaultsOnly, Category = "Magazine")
		FName MagazineAttachSocketName;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Fire")
		FName EjectBoneName;

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
		class UParticleSystem* EjectParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
		FName MuzzleBoneName;

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
		class UParticleSystem* FlashParticle;

	//사운드

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
		class USoundWave* FireSound;



	UPROPERTY(EditDefaultsOnly, Category = "Fire")
		TSubclassOf<class ACBullet> BulletClass;

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
		TSubclassOf<class UMatineeCameraShake> CameraShakeClass;
	UPROPERTY(EditDefaultsOnly, Category = "Fire")
		float ConeAngle = 1.5f;

	//UPROPERTY(EditDefaultsOnly, Category = "Fire")
	//	bool bAvailableAutoFire;

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
		float AutoFireInterval = 0.15f;

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
		float PitchRate = 1.0f;		//반동 속도

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
		float LimitPitchRate = 0.5f;	//최대 누적 반동각



	UPROPERTY(EditDefaultsOnly, Category = "Fire")
		float SpreadInterpSpeed = 20.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
		float MaxSpreadRange = 2.0f;


	UPROPERTY(EditDefaultsOnly, Category = "Trace")
		float HitDistance = 3000;

	UPROPERTY(EditDefaultsOnly, Category = "Hit")
		class UMaterialInstanceConstant* HitDecal;

	UPROPERTY(EditDefaultsOnly, Category = "Hit")
		class UParticleSystem* HitParticle;

	//WC로 갈예정
protected:
	UPROPERTY(EditDefaultsOnly, Category = "HUD")
		TSubclassOf<class UCUserWidget_CrossHair> CrossHairClass;
//	UPROPERTY(EditDefaultsOnly, Category = "HUD")
//		TSubclassOf<class UCUserWidget_HUD> HUDClass;

protected:
	//Rifle Mesh, RootComponent
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Root;

	UPROPERTY(VisibleDefaultsOnly)
		class USkeletalMeshComponent* Mesh;

private:
	//커브 이용을 위한 변수
	UPROPERTY(VisibleDefaultsOnly)
		class UTimelineComponent* Timeline;

public:
	FORCEINLINE bool IsInAim() { return bInAim; }
	FORCEINLINE bool IsFiring() { return bFiring; }
	FORCEINLINE bool IsAutoFire() { return bAutoFire; }

	FORCEINLINE FVector GetLeftHandLocation() {return LeftHandLocation;}
	//복사를 줄이는 방법
	//FORCEINLINE const FVector GetLeftHandLocation() {return LeftHandLocation;}

	FORCEINLINE uint8 GetCurrMagazineCount() { return CurrMagazineCount; }
	FORCEINLINE uint8 GetMaxMagazineCount() { return MaxMagazineCount; }

public:	
	ACWeapon();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;



public:
	bool CanEquip();
	void Equip();
	void Begin_Equip();
	void End_Equip();

	bool CanUnequip();
	void Unequip();

public:
	bool CanAim();
	void Begin_Aim();
	void End_Aim();

public:
	bool CanFire();
	void Begin_Fire();
	void End_Fire();

private:
	UFUNCTION()
		void OnFiring();

public:
	bool CanReload();

	void Reload();


	void Eject_Magazine();
	void Spawn_Magazine();
	void Load_Magazine();
	void End_Reload();

	public:
		void ToggleAutoFire();



private:
	UFUNCTION()
		void OnAiming(float Output);

private:
	class ACPlayer* Owner;


private:
	bool bEquipping;

private:
	bool bInAim;
	bool bFiring;
	bool bReload;
	bool bAutoFire = true;


private:
	FTimerHandle AutoFireHandle;

private:
	float CurrPitchRate;
	float LastAddPitchRate;

private:
	uint8 CurrMagazineCount;
	class ACMagazine* Magazine;


	//WC
protected:
	class UCUserWidget_CrossHair* CrossHair;
//	class UCUserWidget_HUD* HUD;


};
