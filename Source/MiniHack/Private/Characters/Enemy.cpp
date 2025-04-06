#include "Characters/Enemy.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	if (USkeletalMeshComponent* Mesh = GetMesh())
	{
		Mesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
		Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		Mesh->SetGenerateOverlapEvents(true);
	}

	if (UCapsuleComponent Capsule = GetCapsuleComponent())
	{
		Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	}
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

