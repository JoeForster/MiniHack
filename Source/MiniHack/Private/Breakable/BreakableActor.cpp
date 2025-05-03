// Fill out your copyright notice in the Description page of Project Settings.

#include "Breakable/BreakableActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Items/Treasure.h"
#include "Components/CapsuleComponent.h"

ABreakableActor::ABreakableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("Geometry Collection"));
	SetRootComponent(GeometryCollection);
	GeometryCollection->SetGenerateOverlapEvents(true);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(GetRootComponent());
	Capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
}

void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABreakableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//void ABreakableActor::ReceiveHit_Implementation(const FVector& ImpactPoint)
//{
//	void ABreakableActor::ReceiveHit_Implementation(const FVector& ImpactPoint)
//}

void ABreakableActor::OnDestroyed()
{
	// TODO double-check this guard is still needed (was for when we were taking ReceiveHit)
	if (IsBroken)
	{
		return;
	}
	IsBroken = true;

	// TODO should be on destroyed not on every hit.
	// (use OnChaosBreakEvent or is there a better damage callback?)
	if (!TreasureClasses.IsEmpty())
	{
		if (UWorld* World = GetWorld())
		{
			FVector SpawnLocation = GetActorLocation();
			SpawnLocation.Z += 75.f;
			const int32 Selection = FMath::RandRange(0, TreasureClasses.Num() - 1);
			World->SpawnActor<ATreasure>(TreasureClasses[Selection], SpawnLocation, GetActorRotation());
		}
	}
}