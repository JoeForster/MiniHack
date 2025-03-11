// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "HackAnimInstance.generated.h"

class ARogueCharacter;
class UCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class MINIHACK_API UHackAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	ARogueCharacter* RogueCharacter;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	UCharacterMovementComponent* RogueCharacterMovement;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float GroundSpeed;
	
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool JustJumped;
	
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool IsFalling;
};
