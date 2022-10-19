

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



	//FWeaponAimData 지정

	BaseData.TargetArmLength = 200;
	BaseData.SocketOffset = FVector(0, 50, 15);
	BaseData.FieldOfView = 90;

	//Aim
	AimData.TargetArmLength = 30;
	AimData.SocketOffset = FVector(-55, 0, 10);
	//AimData.FieldOfView = 55;
	AimData.FieldOfView = 55;


	LeftHandLocation = FVector(0.0f, 15.f, 0.0f);


	//총의 본 이름
	MagazineBoneName = "b_gun_mag";
	MagazineAttachSocketName = "Pistol_Magazine"; //캐릭터임

	EjectBoneName = "b_gun_shelleject";
	MuzzleBoneName = "Muzzle";

	CHelpers::GetClass<UMatineeCameraShake>(&CameraShakeClass, "Blueprint'/Game/Guns/BP_CameraShake_Pistol.BP_CameraShake_Pistol_C'");

	CHelpers::GetClass<ACMagazine>(&MagazineClass, "Blueprint'/Game/Guns/BP_CMagazine_Pistol.BP_CMagazine_Pistol_C'");

	CHelpers::GetAsset<UAnimMontage>(&ReloadMontage, "AnimMontage'/Game/Character/Animations/Pistol_Reload_Montage.Pistol_Reload_Montage'");

	CHelpers::GetAsset<UAnimMontage>(&EquipMontage, "AnimMontage'/Game/Character/Animations/Pistol_Equip_Montage.Pistol_Equip_Montage'");

	//Character의 본 이름
	RightHandSocketName = "Pistol_RightHand";

	ArmsMeshTransform.SetLocation(FVector(0, 5.1f, -156.6));
	ArmsMeshTransform.SetRotation(FQuat(FRotator(0, -4.8f, 0)));

	ArmsLeftHandTransform.SetLocation(FVector(0, 11, 0));
	ArmsLeftHandTransform.SetRotation(FQuat(FRotator(0, 180, 180)));


	RecoilAngle = 1.5f;
	MaxSpreadAlignment = 4;
	SpreadSpeed = 4;

	MaxMagazineCount = 5;

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

	Owner->GetArms()->SetRelativeTransform(ArmsLeftHandTransform);

	//if (LeftHandSocketName.IsValid())
	//	AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), LeftHandSocketName);

}

void ACWeapon_Pistol::Begin_Aim()
{

}
void ACWeapon_Pistol::End_Aim()
{

}