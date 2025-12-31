#include "Characters/Enemy.h"

#include "Animation/AnimMontage.h"
#include "Components/AttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "HUD/HealthBarComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MiniHack/DebugMacros.h"

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

	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("Health Bar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	if (HealthBarWidget && Attributes)
	{
		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
	}
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

void AEnemy::PlayDeathMontage()
{
	if (USkeletalMeshComponent* CharacterMesh = GetMesh())
	{
		UAnimInstance* AnimInstance = CharacterMesh->GetAnimInstance();
		if (AnimInstance && DeathMontage)
		{
			AnimInstance->Montage_Play(DeathMontage);
			const int32 Selection = FMath::RandRange(0, 2);
			FName SectionName = FName();
			switch (Selection)
			{
			case 0:
				SectionName = FName("Death1");
				break;
			case 1:
				SectionName = FName("Death2");
				break;
			case 2:
				SectionName = FName("Death3");
				break;
			}
			AnimInstance->Montage_JumpToSection(SectionName, DeathMontage);
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
	if (Attributes && Attributes->IsAlive())
	{
		PlayHitReactMontage();
	}
	else
	{
		PlayDeathMontage();
	}

	// TODO play death anim if not alive

	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
	}

	if (HitParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, ImpactPoint);
	}
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Attributes)
	{
		Attributes->ReceiveDamage(DamageAmount);
		if (HealthBarWidget)
		{
			HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
		}
	}

    return DamageAmount;
}

