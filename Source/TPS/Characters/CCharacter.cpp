

#include "Characters/CCharacter.h"
#include "Global.h"

#include "CAnimInstance.h"
#include "Weapons/CWeaponComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


ACCharacter::ACCharacter()
{

	CHelpers::CreateActorComponent<UCWeaponComponent>(this, &Weapon, "Weapon");

	//???
	USkeletalMesh* mesh;
	//SkeletalMesh'/Game/Character/Mesh/SK_Mannequin.SK_Mannequin'
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Character/Mesh/SK_Mannequin.SK_Mannequin'");
	GetMesh()->SetSkeletalMesh(mesh);
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	//AnimInstance ????
	TSubclassOf<UCAnimInstance> animInstance;
	CHelpers::GetClass<UCAnimInstance>(&animInstance, "AnimBlueprint'/Game/Player/ABP_Player.ABP_Player_C'");
	GetMesh()->SetAnimClass(animInstance);

	GetCharacterMovement()->MaxWalkSpeed = 300;

}

void ACCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

