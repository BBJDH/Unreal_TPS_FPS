

#include "Characters/CPlayer.h"
#include "Global.h"


#include "Weapons/CWeaponComponent.h"
#include "CAnimInstance.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"



ACPlayer::ACPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	//스프링, 카메라 생성
	CHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", GetMesh());
	CHelpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera", SpringArm);

	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &Arms, "Arms", Camera);
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &Backpack, "Backpack", GetMesh(), "Backpack");


	CHelpers::CreateActorComponent<UCWeaponComponent>(this, &Weapon, "Weapon");


	//메시
	USkeletalMesh* mesh;
	//SkeletalMesh'/Game/Character/Mesh/SK_Mannequin.SK_Mannequin'
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Character/Mesh/SK_Mannequin.SK_Mannequin'");
	GetMesh()->SetSkeletalMesh(mesh);
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	//AnimInstance 결합
	TSubclassOf<UCAnimInstance> animInstance;
	CHelpers::GetClass<UCAnimInstance>(&animInstance, "AnimBlueprint'/Game/ABP_Player.ABP_Player_C'");
	GetMesh()->SetAnimClass(animInstance);


	GetCharacterMovement()->MaxWalkSpeed = 300;

	//SpringArm 조정, 총에 따라, 모드에 따라 변경
	SpringArm->SetRelativeLocation(FVector(0, 0, 150));
	SpringArm->SetRelativeRotation(FRotator(0, 90, 0));
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bDoCollisionTest = false;




	//카메라
	Camera->SetRelativeLocation(FVector(-30, 0, 0));
	Camera->bUsePawnControlRotation = false;		//지금은 의미없음



	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Character_Arms/Mesh/SK_Mannequin_Arms.SK_Mannequin_Arms'");
	Arms->SetSkeletalMesh(mesh);
	//임의값
	Arms->SetRelativeLocation(FVector(-14.25f, -5.88f, -156.9f));
	Arms->SetRelativeRotation(FRotator(-0.5f, -11.85f, -1.2f));
	Arms->SetVisibility(false);

	UStaticMesh* staticMesh;
	CHelpers::GetAsset<UStaticMesh>(&staticMesh, "StaticMesh'/Game/Weapons/Backpack/Backpack.Backpack'");
	Backpack->SetStaticMesh(staticMesh);
}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();

	NotUseControlRotation();

	APlayerCameraManager* cameraMgr = GetController<APlayerController>()->PlayerCameraManager;
	cameraMgr->ViewPitchMin = PitchRange.X;
	cameraMgr->ViewPitchMax = PitchRange.Y;

}

void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator prev = FRotator(AimPitch, AimYaw, 0);

	//A,B A-B의 각도 , A 컨트롤로테이션, B액터 로테이션
	//-180 ~ 180으로 정규화
	FRotator curr = UKismetMathLibrary::NormalizedDeltaRotator(GetControlRotation(), GetActorRotation());

	//SLerp 쿼터니온의 Lerp
	FRotator delta = UKismetMathLibrary::RInterpTo(prev,curr,DeltaTime,ViewInterpSpeed);

	AimPitch = FMath::Clamp(delta.Pitch, PitchRange.X, PitchRange.Y);
	AimPitch = FMath::Clamp(delta.Pitch, YawRange.X, YawRange.Y);
}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	//입력 함수 바인딩
	PlayerInputComponent->BindAxis("MoveForward", this, &ACPlayer::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACPlayer::OnMoveRight);
	PlayerInputComponent->BindAxis("VerticalLook", this, &ACPlayer::OnVerticalLook);
	PlayerInputComponent->BindAxis("HorizontalLook", this, &ACPlayer::OnHorizontalLook);


	PlayerInputComponent->BindAction("AR4", IE_Pressed, Weapon, &UCWeaponComponent::SetAR4Mode);
	PlayerInputComponent->BindAction("AK47", IE_Pressed, Weapon, &UCWeaponComponent::SetAK47Mode);
	PlayerInputComponent->BindAction("Pistol", IE_Pressed, Weapon, &UCWeaponComponent::SetPistolMode);

	PlayerInputComponent->BindAction("Aim", IE_Pressed, Weapon, &UCWeaponComponent::Begin_Aim);
	PlayerInputComponent->BindAction("Aim", IE_Released, Weapon, &UCWeaponComponent::End_Aim);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, Weapon, &UCWeaponComponent::Begin_Fire);
	PlayerInputComponent->BindAction("Fire", IE_Released, Weapon, &UCWeaponComponent::End_Fire);

	PlayerInputComponent->BindAction("AutoFire", IE_Pressed, Weapon, &UCWeaponComponent::ToggleAutoFire);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, Weapon, &UCWeaponComponent::Reload);


}

void ACPlayer::OnMoveForward(float AxisValue)
{
	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetForwardVector().GetSafeNormal2D();

	AddMovementInput(direction, AxisValue);
}

void ACPlayer::OnMoveRight(float AxisValue)
{
	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetRightVector().GetSafeNormal2D();

	AddMovementInput(direction, AxisValue);
}

void ACPlayer::OnVerticalLook(float AxisValue)
{
	FRotator controlRotation = GetControlRotation().GetNormalized();
	//CLog::Log("dd");
	float limitPitch = controlRotation.Pitch;

	CheckTrue(limitPitch > LimitPitchAngle);
	AddControllerPitchInput(AxisValue);
}

void ACPlayer::OnHorizontalLook(float AxisValue)
{
	AddControllerYawInput(AxisValue);
}

void ACPlayer::UseControlRotation()
{
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
}

void ACPlayer::NotUseControlRotation()
{
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}