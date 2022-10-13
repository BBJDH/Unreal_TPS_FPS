

#include "Weapons/CWeapon.h"
#include "Global.h"
#include "CBullet.h"
#include "CMagazine.h"
#include "Components/SkeletalMeshComponent.h"
//#include "GameFramework/Character.h"
#include "Characters/CPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Widgets/CUserWidget_CrossHair.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundWave.h"
#include "Camera/CameraShake.h"
#include "Animation/AnimMontage.h"


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




ACWeapon::ACWeapon()
{
	PrimaryActorTick.bCanEverTick = true;


	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &Mesh, "Mesh", Root);

	CHelpers::CreateActorComponent<UTimelineComponent>(this, &Timeline, "Timeline");

	CHelpers::GetAsset<UCurveFloat>(&AimCurve, "CurveFloat'/Game/CV_Aim.CV_Aim'");
	CHelpers::GetAsset<UMaterialInstanceConstant>(&HitDecal, "MaterialInstanceConstant'/Game/Materials/M_Decal_Inst.M_Decal_Inst'");
	CHelpers::GetAsset<UParticleSystem>(&HitParticle, "ParticleSystem'/Game/Effects/P_Impact_Default.P_Impact_Default'");


	CHelpers::GetClass<UCUserWidget_CrossHair>(&CrossHairClass, "WidgetBlueprint'/Game/Widgets/WB_CrossHair.WB_CrossHair_C'");

	CHelpers::GetAsset<UParticleSystem>(&EjectParticle, "ParticleSystem'/Game/Effects/P_Eject_bullet.P_Eject_bullet'");
	CHelpers::GetAsset<UParticleSystem>(&FlashParticle, "ParticleSystem'/Game/Effects/P_Muzzleflash.P_Muzzleflash'");

	CHelpers::GetAsset<USoundWave>(&FireSound, "SoundWave'/Game/Sounds/S_RifleShoot.S_RifleShoot'");


	CHelpers::GetClass<ACBullet>(&BulletClass, "Blueprint'/Game/Guns/BP_CBullet.BP_CBullet_C'");

}

void ACWeapon::BeginPlay()
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
		//배속(커브가 20초짜리)
		Timeline->SetPlayRate(AimingSpeed);
	}
	

	if (!!CrossHairClass)
	{
		CrossHair = CreateWidget<UCUserWidget_CrossHair, APlayerController>
			(
				Owner->GetController<APlayerController>(),
				CrossHairClass									//생성할 타입
				);
		CrossHair->AddToViewport();								//위젯 뷰포트에 보여지도록
		CrossHair->SetVisibility(ESlateVisibility::Hidden);
	}

	CurrMagazineCount = MaxMagazineCount;

	//장착
	if (HolsterSocketName.IsValid())
		AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), HolsterSocketName);

	//UpdateSpreadRange();
}

void ACWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (LastAddSpreadTime >= 0.0f)
	{
		if (GetWorld()->GetTimeSeconds() - LastAddSpreadTime >= AutoFireInterval + 0.25f)
		{
			CurrSpreadRadius = 0;
			LastAddSpreadTime = 0;

			CrossHair->UpdateSpreadRange(CurrSpreadRadius, MaxSpreadAlignment);
		}
	}

}
bool ACWeapon::CanEquip()
{
	//장착이 안될조건을 다 적어놓고 뒤집기
	
	bool b = false;
	b |= bEquipping;
	b |= bReload;
	b |= bFiring;


	return !b;
	
}
void ACWeapon::Equip()
{
	bEquipping = true;
	if (!!EquipMontage)
		Owner->PlayAnimMontage(EquipMontage, EquipMontage_PlayRate);

	if (!!CrossHair)
		CrossHair->SetVisibility(ESlateVisibility::Visible);
}
void ACWeapon::Begin_Equip()
{
	if (RightHandSocketName.IsValid())
		AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), RightHandSocketName);

}
void ACWeapon::End_Equip() 
{
	bEquipping = false;

}

bool ACWeapon::CanUnequip()
{
	//장착이 안될조건을 다 적어놓고 뒤집기

	bool b = false;
	b |= bEquipping;
	b |= bReload;


	return !b;

}


void ACWeapon::Unequip()
{
	End_Aim();
	End_Fire();
	if (HolsterSocketName.IsValid())
		AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), HolsterSocketName);
	//AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), HolsterSocketName);
	if(!!CrossHair)
		CrossHair->SetVisibility(ESlateVisibility::Hidden);
}

bool ACWeapon::CanAim()
{
	bool b = false;
	b |= bEquipping;
	b |= bReload;
	b |= bInAim;

	return !b;
}

void ACWeapon::Begin_Aim()
{

	bInAim = true;

	if (!!AimCurve)
	{
		//TimelineStart "OnAiming" Call
		Timeline->PlayFromStart();
		AimData.SetData(Owner);

		return;
	}

	AimData.SetDataByNoneCurve(Owner);
}

void ACWeapon::End_Aim()
{
	CheckFalse(bInAim);
	bInAim = false;

	if (!!AimCurve)
	{
		//TimelineReverse "OnAiming" Call
		Timeline->ReverseFromEnd();
		BaseData.SetData(Owner);

		return;
	}

	BaseData.SetDataByNoneCurve(Owner);
}


//Output : 1~0
void ACWeapon::OnAiming(float Output)
{
	UCameraComponent* camera = CHelpers::GetComponent<UCameraComponent>(Owner);
	camera->FieldOfView = FMath::Lerp<float>(AimData.FieldOfView, BaseData.FieldOfView, Output);
}

bool ACWeapon::CanFire()
{
	//안되는 상황을 모두 or연산하고 마지막에 뒤집음
	bool b = false;
	b |= bEquipping;
	b |= bReload;
	b |= bFiring;


	return !b;

}

void ACWeapon::Begin_Fire()
{

	bFiring = true;

	if (bAutoFire)
	{
		GetWorld()->GetTimerManager().SetTimer(AutoFireHandle, this, &ACWeapon::OnFiring, AutoFireInterval, true, 0);

		return;
	}

	OnFiring();
}

void  ACWeapon::End_Fire()
{
	if (bAutoFire)
		GetWorld()->GetTimerManager().ClearTimer(AutoFireHandle);

	bFiring = false;
}
void ACWeapon::OnFiring()
{
	//카메라의 위치에서 화면 중앙으로 발사
	UCameraComponent* camera = CHelpers::GetComponent<UCameraComponent>(Owner);
	FVector direction = camera->GetForwardVector();
	FTransform transform = camera->GetComponentToWorld();

	FVector start = transform.GetLocation() + direction;


	//랜덤 탄착군 생성
	direction = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(direction, RecoilAngle);

	FVector end = transform.GetLocation() + direction * HitDistance;

	//DrawDebugLine(GetWorld(), start, end, FColor::Green, true, 2);


	TArray<AActor*> ignores;
	ignores.Add(Owner);

	FHitResult hitResult;
	UKismetSystemLibrary::LineTraceSingle
	(
		GetWorld(),
		start,
		end,
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ignores,
		EDrawDebugTrace::None,
		hitResult,
		true
	);
	if (hitResult.bBlockingHit)
	{
		//CLog::Print(hitResult.GetActor()->GetName());
		FRotator rotator = hitResult.ImpactNormal.Rotation();

		if (!!HitDecal)
		{
			UDecalComponent* decal = UGameplayStatics::SpawnDecalAtLocation
			(
				GetWorld(),
				HitDecal,
				FVector(5),
				hitResult.Location,
				rotator,
				10
			);
			decal->SetFadeScreenSize(0);	//LOD 끄기 (화면사이즈에 대해 LOD를 잡으라는 뜻, 화면사이즈를 0으로 잡음)
		}
		if (!!HitParticle)
		{
			FRotator impactRotation = UKismetMathLibrary::FindLookAtRotation(hitResult.Location, hitResult.TraceStart);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, hitResult.Location, impactRotation);
		}

	}



	if (!!FlashParticle)
		UGameplayStatics::SpawnEmitterAttached(FlashParticle, Mesh, MuzzleBoneName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset);


	if (!!EjectParticle)
		UGameplayStatics::SpawnEmitterAttached(EjectParticle, Mesh, EjectBoneName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset);

	FVector muzzleLocation = Mesh->GetSocketLocation(MuzzleBoneName);
	if (!!FireSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, muzzleLocation);
	//CurrPitchRate += PitchRate * GetWorld()->GetDeltaSeconds();
	//if (CurrPitchRate <= LimitPitchRate)
	//	Owner->AddControllerPitchInput(-CurrPitchRate);

	//UpdateSpreadRange();

	if (!!BulletClass)
	{
		FVector spawnLocation = muzzleLocation + direction * 100;


		ACBullet* bullet = GetWorld()->SpawnActor<ACBullet>(BulletClass, spawnLocation, direction.Rotation());
		bullet->Shoot(direction);
	}

	Owner->AddControllerPitchInput(-RecoilRate * UKismetMathLibrary::RandomFloatInRange(0.8f, 1.2f));

	if (!!CameraShakeClass)
	{
		APlayerController* controller = Owner->GetController<APlayerController>();
		if (!!controller)
		{
			controller->PlayerCameraManager->StartMatineeCameraShake(CameraShakeClass);
		}
	}
	//0~ 360을 



	if (CurrSpreadRadius <= 1.0f)
	{
		CurrSpreadRadius += SpreadSpeed * GetWorld()->GetDeltaSeconds();

		CrossHair->UpdateSpreadRange(CurrSpreadRadius, MaxSpreadAlignment);
	}
	LastAddSpreadTime = GetWorld()->GetTimeSeconds();


	//Timer는 쓰레드 안에서 동작시키면 안된다 별도의 쓰레드로 작성시켜야함
	//LastAddPitchRate = GetWorld()->GetTimeSeconds();

	CurrMagazineCount--;


	if (CurrMagazineCount <= 0)
		Reload();

}

bool ACWeapon::CanReload()
{
	bool b = false;
	b |= bEquipping;
	b |= bReload;


	return !b;
}

void ACWeapon::Reload()
{
	//AnimMontage'/Game/Character/Animations/Rifle_Jog_Reload_Montage.Rifle_Jog_Reload_Montage'

	CheckTrue(bReload);
	bReload = true;

	End_Aim();
	End_Fire();

	if (!!ReloadMontage)
	{
		Owner->PlayAnimMontage(ReloadMontage);
	}
}


void ACWeapon::Eject_Magazine()
{
	if(MagazineBoneName.IsValid())
		Mesh->HideBoneByName(MagazineBoneName, PBO_None);

	CheckNull(MagazineClass);

	FTransform transform = Mesh->GetSocketTransform(MagazineBoneName);
	ACMagazine* magazine = GetWorld()->SpawnActorDeferred<ACMagazine>(MagazineClass, transform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	magazine->Eject();
	magazine->SetLifeSpan(5);
	magazine->FinishSpawning(transform);
}

void ACWeapon::Spawn_Magazine()
{
	CheckNull(MagazineClass);
	Magazine = GetWorld()->SpawnActor<ACMagazine>(MagazineClass);
	Magazine->AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), MagazineAttachSocketName);
}

void ACWeapon::Load_Magazine()
{
	if (!!Magazine)
		Magazine->Destroy();


	if(MagazineBoneName.IsValid())
		Mesh->UnHideBoneByName(MagazineBoneName);

	CurrMagazineCount = MaxMagazineCount;

}

void ACWeapon::End_Reload()
{
	bReload = false;
}

void ACWeapon::ToggleAutoFire()
{
	if (bAutoFire && bFiring)
		End_Fire();

	bAutoFire = !bAutoFire;
}


