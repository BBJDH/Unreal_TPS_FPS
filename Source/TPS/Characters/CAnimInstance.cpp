

#include "Characters/CAnimInstance.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Characters/CPlayer.h"
//#include "Weapons/CRifle.h"
#include "Misc/CString.h"

void UCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	Owner = Cast<ACPlayer>(TryGetPawnOwner());
	CheckNull(Owner);
}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	CheckNull(Owner);

	Speed = Owner->GetVelocity().Size2D();
	Direction = CalculateDirection(Owner->GetVelocity(), Owner->GetActorRotation());

	AimPitch = Owner->GetAimPitch();
	AimYaw = Owner->GetAimYaw();

	//bInAim = Owner->IsInAim();
	//ACRifle* rifle = Owner->GetRifle();
	//if (!!rifle)
	//{
	//	bInAim = rifle->IsInAim();
	//	bFiring = rifle->IsFiring();
	//}

	FString Str{};
	
}

