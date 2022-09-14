

#include "Weapons/CRifle.h"
#include "Global.h"

#include "Components/SkeletalMeshComponent.h"
//#include "GameFramework/Character.h"
#include "Characters/CPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "Camera/CameraComponent.h"

void FWeaponAimData::SetData(ACharacter* InOwner)
{
	USpringArmComponent* springArm = CHelpers::GetComponent<USpringArmComponent>(InOwner);
	springArm->TargetArmLength = TargetArmLength;
	springArm->SocketOffset = SocketOffset;
}

void FWeaponAimData::SetDataByNoneCurve(ACharacter* InOwner)
{
	USpringArmComponent* springArm = CHelpers::GetComponent<USpringArmComponent>(InOwner);
	springArm->TargetArmLength = TargetArmLength;
	springArm->SocketOffset = SocketOffset;

	UCameraComponent* camera = CHelpers::GetComponent<UCameraComponent>(InOwner);
	camera->FieldOfView = FieldOfView;
}

/*=====================여기까지 구조체 정의========================*/

ACRifle::ACRifle()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &Mesh, "Mesh", Root);


	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Weapons/Meshes/Ka47/SK_KA47.SK_KA47'");
	Mesh->SetSkeletalMesh(mesh);

	BaseData.TargetArmLength = 200;
	BaseData.SocketOffset = FVector(0, 50, 15);
	BaseData.FieldOfView = 90;

	AimData.TargetArmLength = 80;
	AimData.SocketOffset = FVector(0, 55, 10);
	AimData.FieldOfView = 65;

	CHelpers::CreateActorComponent<UTimelineComponent>(this, &Timeline, "Timeline");

	CHelpers::GetAsset<UCurveFloat>(&AimCurve,"CurveFloat'/Game/CV_Aim.CV_Aim'");
}

void ACRifle::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ACPlayer>(GetOwner());
	CheckNull(Owner);

	BaseData.SetDataByNoneCurve(Owner);

	if (!!AimCurve)
	{
		FOnTimelineFloat timeline;
		timeline.BindUFunction(this, "OnAiming");

		Timeline->AddInterpFloat(AimCurve, timeline);
		Timeline->SetLooping(false);
		Timeline->SetPlayRate(AimingSpeed);
	}
}

void ACRifle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACRifle::OnAim()
{
	CheckTrue(bInAim);
	bInAim = true;

	if (!!AimCurve)
	{
		Timeline->PlayFromStart();
		AimData.SetData(Owner);

		return;
	}

	AimData.SetDataByNoneCurve(Owner);
}

void ACRifle::OffAim()
{
	CheckFalse(bInAim);
	bInAim = false;

	if (!!AimCurve)
	{
		Timeline->ReverseFromEnd();
		BaseData.SetData(Owner);

		return;
	}

	BaseData.SetDataByNoneCurve(Owner);
}


//Output : 1~0
void ACRifle::OnAiming(float Output)
{
	UCameraComponent* camera = CHelpers::GetComponent<UCameraComponent>(Owner);
	camera->FieldOfView = FMath::Lerp<float>(AimData.FieldOfView, BaseData.FieldOfView, Output);
}

void ACRifle::Firing()
{
	UCameraComponent* camera = CHelpers::GetComponent<UCameraComponent>(Owner);
	FVector direction = camera->GetForwardVector();
	FTransform transform = camera->GetComponentToWorld();

	FVector start = transform.GetLocation() + direction;

	direction = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(direction, ConeAngle);
	FVector end = transform.GetLocation() + direction * HitDistance;

	DrawDebugLine(GetWorld(), start, end, FColor::Green, true, 2);
}