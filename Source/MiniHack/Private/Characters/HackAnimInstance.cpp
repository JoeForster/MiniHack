// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/HackAnimInstance.h"
#include "Characters/RogueCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UHackAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	RogueCharacter = Cast<ARogueCharacter>(TryGetPawnOwner());
	if (RogueCharacter)
	{
		RogueCharacterMovement = RogueCharacter->GetCharacterMovement();
	}

}

void UHackAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (RogueCharacter)
	{
		// Assumes no timed hold-jump or anything like that
		JustJumped = RogueCharacter->bPressedJump;
	}

	if (RogueCharacterMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(RogueCharacterMovement->Velocity);
		IsFalling = RogueCharacterMovement->IsFalling();
	}
}
