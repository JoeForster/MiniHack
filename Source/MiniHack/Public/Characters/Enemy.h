// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitReceiver.h"
#include "Enemy.generated.h"

class UAnimMontage;

UCLASS()
class MINIHACK_API AEnemy : public ACharacter, public IHitReceiver
{
	GENERATED_BODY()

public:
	AEnemy();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void ReceiveHit(const FVector& ImpactPoint) override;

protected:
	virtual void BeginPlay() override;
	
	// Montage play functions
	void PlayHitReactMontage();

private:
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere, Category = Sounds)
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = VisualEffects)
	UParticleSystem* HitParticles;
};
