#include "CAnimNotify_Magazine.h"
#include "Global.h"
#include "Characters/CPlayer.h"
//#include "Weapons/CRifle.h"

FString UCAnimNotify_Magazine::GetNotifyName_Implementation() const
{
	return "Magazine";
}

void UCAnimNotify_Magazine::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(player);

	//ACRifle* rifle = player->GetRifle();

	//switch (ActionType)
	//{
	//case EMagazineActionType::Eject: rifle->Eject_Magazine(); break;
	//case EMagazineActionType::Spawn: rifle->Spawn_Magazine(); break;
	//case EMagazineActionType::End: rifle->End_Magazine(); break;
	//}
}