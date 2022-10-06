

#include "Characters/CPlayer.h"
#include "Global.h"


#include "Weapons/CWeaponComponent.h"
#include "CAnimInstance.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"

ACPlayer::ACPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	//������, ī�޶� ����
	CHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", GetMesh());
	CHelpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera", SpringArm);

	CHelpers::CreateActorComponent<UCWeaponComponent>(this, &Weapon, "Weapon");


	//�޽�
	USkeletalMesh* mesh;
	//SkeletalMesh'/Game/Character/Mesh/SK_Mannequin.SK_Mannequin'
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Character/Mesh/SK_Mannequin.SK_Mannequin'");
	GetMesh()->SetSkeletalMesh(mesh);
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	//AnimInstance ����
	TSubclassOf<UCAnimInstance> animInstance;
	CHelpers::GetClass<UCAnimInstance>(&animInstance, "AnimBlueprint'/Game/ABP_Player.ABP_Player_C'");
	GetMesh()->SetAnimClass(animInstance);


	GetCharacterMovement()->MaxWalkSpeed = 300;

	//SpringArm ����, �ѿ� ����, ��忡 ���� ����
	SpringArm->SetRelativeLocation(FVector(0, 0, 150));
	SpringArm->SetRelativeRotation(FRotator(0, 90, 0));
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bDoCollisionTest = false;




	//ī�޶�
	Camera->SetRelativeLocation(FVector(-30, 0, 0));
	Camera->bUsePawnControlRotation = false;		//������ �ǹ̾���
}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();

	//if (!!RifleClass)
	//{
	//	FActorSpawnParameters params;
	//	params.Owner = this;


	//	//������ Ÿ������ �� ����
	//	Rifle = GetWorld()->SpawnActor<ACRifle>(RifleClass, params);
	//	Rifle->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), "Rifle_RightHand");

	//}

}

void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator prev = FRotator(AimPitch, AimYaw, 0);

	//A,B A-B�� ���� , A ��Ʈ�ѷ����̼�, B���� �����̼�
	//-180 ~ 180���� ����ȭ
	FRotator curr = UKismetMathLibrary::NormalizedDeltaRotator(GetControlRotation(), GetActorRotation());

	//SLerp ���ʹϿ��� Lerp
	FRotator delta = UKismetMathLibrary::RInterpTo(prev,curr,DeltaTime,ViewInterpSpeed);

	AimPitch = FMath::Clamp(delta.Pitch, PitchRange.X, PitchRange.Y);
	AimPitch = FMath::Clamp(delta.Pitch, YawRange.X, YawRange.Y);
}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	//�Է� �Լ� ���ε�
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