
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CMagazine.generated.h"

UCLASS()
class TPS_API ACMagazine : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* Mesh;
	
public:	
	ACMagazine();

protected:
	virtual void BeginPlay() override;

public:
	void EnablePhysics();

};
