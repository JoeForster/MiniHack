// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HitReceiver.h"
#include "BreakableActor.generated.h"

class UGeometryCollectionComponent;

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

private:
	UPROPERTY(VisibleAnywhere)
	UGeometryCollectionComponent* GeometryCollection;

};
