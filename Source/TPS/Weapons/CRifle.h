
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CRifle.generated.h"


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
class TPS_API ACRifle : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Data")
		FWeaponAimData BaseData;

	UPROPERTY(EditDefaultsOnly, Category = "Data")
		FWeaponAimData AimData;

	UPROPERTY(EditDefaultsOnly, Category = "Aiming")
		class UCurveFloat* AimCurve;

	UPROPERTY(EditDefaultsOnly, Category = "Aiming")
		float AimingSpeed = 200;

	UPROPERTY(EditDefaultsOnly, Category = "Hit")
		float HitDistance = 3000;

	UPROPERTY(EditDefaultsOnly, Category = "Hit")
		float ConeAngle = 0.75f;

private:
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
	ACRifle();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;


public:
	void OnAim();
	void OffAim();

	void Firing();

private:
	UFUNCTION()
		void OnAiming(float Output);

private:
	class ACPlayer* Owner;

private:
	bool bInAim;
};
