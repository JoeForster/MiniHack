#include "Characters/Enemy.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "MiniHack/DebugMacros.h"
#include "Animation/AnimMontage.h"
#include "Kismet/GameplayStatics.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	if (USkeletalMeshComponent* MyMesh = GetMesh())
	{
		MyMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		MyMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
		MyMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		MyMesh->SetGenerateOverlapEvents(true);
	}

	if (UCapsuleComponent* Capsule = GetCapsuleComponent())
	{
		Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	}
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemy::PlayHitReactMontage()
{
	if (USkeletalMeshComponent* CharacterMesh = GetMesh())
	{
		UAnimInstance* AnimInstance = CharacterMesh->GetAnimInstance();
		if (AnimInstance && HitReactMontage)
		{
			AnimInstance->Montage_Play(HitReactMontage);
			const int32 Selection = FMath::RandRange(0, 1);
			FName SectionName = FName();
			switch (Selection)
			{
			case 0:
				SectionName = FName("Hit1");
				break;
			case 1:
				SectionName = FName("Hit2");
				break;
			}
			AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
		}
	}
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::ReceiveHit_Implementation(const FVector& ImpactPoint)
{
	//DRAW_SPHERE(ImpactPoint);
	//auto* world = GetWorld();
	//if (world)
	//{
	//	DrawDebugSphere(GetWorld(), ImpactPoint, 5.f, 10, FColor::Emerald, true);
	//}
	PlayHitReactMontage();

	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
	}

	if (HitParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, ImpactPoint);
	}
}

