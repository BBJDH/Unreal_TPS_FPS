

#include "Weapons/CMagazine.h"
#include "Global.h"
#include "Components/StaticMeshComponent.h"


ACMagazine::ACMagazine()
{

	//StaticMesh'/Game/Weapons/Meshes/Ka47/SM_KA47_Mag.SM_KA47_Mag'
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &Mesh, "Mesh");

	UStaticMesh* mesh;
	CHelpers::GetAsset<UStaticMesh>(&mesh, "StaticMesh'/Game/Weapons/Meshes/Ka47/SM_KA47_Mag.SM_KA47_Mag'");
	Mesh->SetStaticMesh(mesh);
	

	Mesh->SetCollisionProfileName("Magazine");


}

void ACMagazine::BeginPlay()
{
	Super::BeginPlay();
	
}


void ACMagazine::EnablePhysics() 
{
	Mesh->SetSimulatePhysics(true);
}