

#include "Weapons/CMagazine.h"
#include "Global.h"
#include "Components/StaticMeshComponent.h"


ACMagazine::ACMagazine()
{

	//StaticMesh'/Game/Weapons/Meshes/Ka47/SM_KA47_Mag.SM_KA47_Mag'
	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &Mesh, "Mesh", Root);
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &Mesh_Empty, "Mesh_Empty", Root);

	Mesh->SetCollisionProfileName("Magazine");
	Mesh_Empty->SetCollisionProfileName("Magazine");


}

void ACMagazine::BeginPlay()
{
	Super::BeginPlay();
	if (bEject == false)
	{
		Mesh->SetVisibility(true);
		Mesh_Empty->SetVisibility(false);
	}
}


void ACMagazine::Eject()
{
	bEject = true;

	Mesh->SetVisibility(false);
	Mesh_Empty->SetSimulatePhysics(true);
}