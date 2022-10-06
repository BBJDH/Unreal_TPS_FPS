#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CBullet.generated.h"

UCLASS()
class TPS_API ACBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	ACBullet();

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UCapsuleComponent* Capsule;

	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* Mesh;
	
	UPROPERTY(VisibleDefaultsOnly)
		class UProjectileMovementComponent* Projectile;

protected:
	virtual void BeginPlay() override;


public:
	void Shoot(const FVector& InDirection);

private:
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
