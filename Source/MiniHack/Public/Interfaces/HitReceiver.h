// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HitReceiver.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHitReceiver : public UInterface
{
	GENERATED_BODY()
};

class MINIHACK_API IHitReceiver
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void ReceiveHit(const FVector& ImpactPoint);
};
