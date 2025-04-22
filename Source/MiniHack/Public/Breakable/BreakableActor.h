// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HitReceiver.h"
#include "BreakableActor.generated.h"

class ATreasure;
class UGeometryCollectionComponent;
class UCapsuleComponent;

UCLASS()
class MINIHACK_API ABreakableActor : public AActor, public IHitReceiver
{
	GENERATED_BODY()
	
public:	
	ABreakableActor();
	virtual void Tick(float DeltaTime) override;
	
	virtual void ReceiveHit_Implementation(const FVector& ImpactPoint) override;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCapsuleComponent* Capsule;


private:
	UPROPERTY(VisibleAnywhere)
	UGeometryCollectionComponent* GeometryCollection;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ATreasure> DroppedOnDestroyed;

	// HACK until I fix infinite loop issue due to treasure colliding with owner
	bool HasSpawnedTreasure = false;

};
