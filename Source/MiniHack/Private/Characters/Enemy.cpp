#include "Characters/Enemy.h"

#include "Animation/AnimMontage.h"
#include "Components/AttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HUD/HealthBarComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MiniHack/DebugMacros.h"
#include "AIController.h"
#include "NavigationPath.h"
#include "Navigation/PathFollowingComponent.h"

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

	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		Movement->bOrientRotationToMovement = true;
		bUseControllerRotationPitch = false;
		bUseControllerRotationYaw = false;
		bUseControllerRotationRoll = false;
	}
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	if (HealthBarWidget && Attributes)
	{
		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
		HealthBarWidget->SetVisibility(false);
	}

	EnemyController = Cast<AAIController>(GetController());
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

void AEnemy::Die()
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
				DeathPose = EDeathPose::EDP_Death1;
				break;
			case 1:
				SectionName = FName("Death2");
				DeathPose = EDeathPose::EDP_Death2;
				break;
			case 2:
				SectionName = FName("Death3");
				DeathPose = EDeathPose::EDP_Death3;
				break;
			}
			
			AnimInstance->Montage_JumpToSection(SectionName, DeathMontage);
		}
	}

	if (UCapsuleComponent* Capsule = GetCapsuleComponent())
	{
		Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	SetLifeSpan(3.0);

	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}
}

bool AEnemy::IsInRangeOfTarget(AActor* Target, double Radius)
{
	if (Target != nullptr)
	{
		const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
		if (GEngine)
		{
			const FString OnScreenString = FString::Printf(TEXT("IsInRangeOfTarget Distance %f = %f?"), DistanceToTarget, Radius);
			GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Green, OnScreenString);
		}
		return (DistanceToTarget <= Radius);
	}
	return false;
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const bool InCombatRange = IsInRangeOfTarget(CombatTarget, CombatRadius);
	if (!InCombatRange)
	{
		CombatTarget = nullptr;
		if (HealthBarWidget)
		{
			HealthBarWidget->SetVisibility(false);
		}
	}
	const AActor* PrevPatrolTarget = nullptr;
	const bool ArrivedAtPatrolPoint = IsInRangeOfTarget(PatrolTarget, PatrolArrivalRadius);
	if (ArrivedAtPatrolPoint)
	{
		PrevPatrolTarget = PatrolTarget;
		PatrolTarget = nullptr;

	}
	if (EnemyController != nullptr && PatrolTarget == nullptr)
	{
		TArray<AActor*> ValidTargets;
		for (AActor* CheckTarget : PatrolPath)
		{
			if (CheckTarget != PatrolTarget && CheckTarget != nullptr)
			{
				ValidTargets.AddUnique(CheckTarget);
			}
		}

		int32 NumValidTargets = ValidTargets.Num();
		if (NumValidTargets > 0)
		{
			const int32 Selection = FMath::RandRange(0, NumValidTargets - 1);
			PatrolTarget = PatrolPath[Selection];

			if (PatrolTarget != nullptr && PatrolTarget != EnemyController)
			{
				if (GEngine)
				{
					const FString OnScreenString = FString::Printf(TEXT("PATROL POINT %i"), Selection);
					GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Green, OnScreenString);
				}

				FAIMoveRequest MoveRequest;
				MoveRequest.SetGoalActor(PatrolTarget);
				MoveRequest.SetAcceptanceRadius(15.f);
				FNavPathSharedPtr NavPath;
				FPathFollowingRequestResult result = EnemyController->MoveTo(MoveRequest, &NavPath);
				if (NavPath)
				{
					TArray<FNavPathPoint>& PathPoints = NavPath->GetPathPoints();
					for (auto Point : PathPoints)
					{
						const FVector& Location = Point.Location;
						DrawDebugSphere(GetWorld(), Location, 12.f, 12, FColor::Green, false, 10.f);	
					}
				}
			}
		}
	}

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

	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(true);
	}

	if (Attributes && Attributes->IsAlive())
	{
		PlayHitReactMontage();
	}
	else
	{
		Die();
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

	CombatTarget = EventInstigator ? EventInstigator->GetPawn() : nullptr;

    return DamageAmount;
}

