

#include "Weapons/CWeapon_Pistol.h"

#include "Global.h"
#include "CMagazine.h"
#include "CWeaponComponent.h"
#include "Characters/CPlayer.h"
#include "Widgets/CUserWidget_CrossHair.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimMontage.h"


ACWeapon_Pistol::ACWeapon_Pistol()
{
	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Weapons/Meshes/Pistol/Pistols_A.Pistols_A'");
	Mesh->SetSkeletalMesh(mesh);



	//FWeaponAimData ����

	BaseData.TargetArmLength = 200;
	BaseData.SocketOffset = FVector(0, 50, 15);
	BaseData.FieldOfView = 90;

	//Aim
	AimData.TargetArmLength = 30;
	AimData.SocketOffset = FVector(-55, 0, 10);
	AimData.FieldOfView = 55;


	LeftHandLocation = FVector(0.0f, 15.f, 0.0f);


	//���� �� �̸�
	MagazineBoneName = "b_gun_mag";
	MagazineAttachSocketName = "Pistol_Magazine"; //ĳ������

	EjectBoneName = "b_gun_shelleject";
	MuzzleBoneName = "Muzzle";

	CHelpers::GetClass<UMatineeCameraShake>(&CameraShakeClass, "Blueprint'/Game/Guns/BP_CameraShake_Pistol.BP_CameraShake_Pistol_C'");

	CHelpers::GetClass<ACMagazine>(&MagazineClass, "Blueprint'/Game/Guns/BP_CMagazine_Pistol.BP_CMagazine_Pistol_C'");

	CHelpers::GetAsset<UAnimMontage>(&ReloadMontage, "AnimMontage'/Game/Character/Animations/Pistol_Reload_Montage.Pistol_Reload_Montage'");

	CHelpers::GetAsset<UAnimMontage>(&EquipMontage, "AnimMontage'/Game/Character/Animations/Pistol_Equip_Montage.Pistol_Equip_Montage'");

	//Character�� �� �̸�
	RightHandSocketName = "Pistol_RightHand";

	ArmsMeshTransform.SetLocation(FVector(0, 5.1f, -156.6));
	ArmsMeshTransform.SetRotation(FQuat(FRotator(0, -4.8f, 0)));

	ArmsLeftHandTransform.SetLocation(FVector(0, 11, 0));
	ArmsLeftHandTransform.SetRotation(FQuat(FRotator(0, 180, 180)));
	//CLog::Print(ArmsLeftHandTransform.GetLocation());
	//CLog::Print(FRotator(ArmsLeftHandTransform.GetRotation()));

	bAutoFire = false;

	RecoilAngle = 1.5f;
	MaxSpreadAlignment = 8;
	SpreadSpeed = 8;

	MaxMagazineCount = 5;

	AutoFireInterval = 0.3f;

}

void ACWeapon_Pistol::BeginPlay()
{
	Super::BeginPlay();
	Mesh->SetVisibility(false);
}

void ACWeapon_Pistol::Begin_Equip()
{
	Super::Begin_Equip();
	Mesh->SetVisibility(true);

	Owner->GetArms()->SetRelativeTransform(ArmsMeshTransform);
}

void ACWeapon_Pistol::Begin_Aim()
{
	Super::Begin_Aim();
	if (!!CrossHair)
		CrossHair->SetVisibility(ESlateVisibility::Hidden);

	Owner->GetArms()->SetVisibility(true);
	Owner->GetMesh()->SetVisibility(false);
	Owner->GetBackpack()->SetVisibility(false);

	AttachToComponent(Owner->GetArms(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), RightHandSocketName);

	CHelpers::GetComponent<UCWeaponComponent>(Owner)->OnWeaponAim_Arms_Begin.Broadcast(this);
	//CLog::Print(ArmsLeftHandTransform.GetLocation());
	//CLog::Print(FRotator(ArmsLeftHandTransform.GetRotation()));
}
void ACWeapon_Pistol::End_Aim()
{
	Super::End_Aim();


	if (!!CrossHair)
		CrossHair->SetVisibility(ESlateVisibility::Visible);

	Owner->GetArms()->SetVisibility(false);
	Owner->GetMesh()->SetVisibility(true);
	Owner->GetBackpack()->SetVisibility(true);

	AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), RightHandSocketName);

	CHelpers::GetComponent<UCWeaponComponent>(Owner)->OnWeaponAim_Arms_End.Broadcast();
}