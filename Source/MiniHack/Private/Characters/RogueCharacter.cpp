#include "Characters/RogueCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"

ARogueCharacter::ARogueCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 450.f, 0.f);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);

}

void ARogueCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARogueCharacter::MoveForward(float Value)
{
	if (Controller != nullptr && Value != 0.f)
	{
		// Get the forward movement vector based on controller's rotation
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
		const FRotationMatrix YawRotator(YawRotation);
		const FVector ControlForward = YawRotator.GetUnitAxis(EAxis::X);
		AddMovementInput(ControlForward, Value);
	}
}

void ARogueCharacter::MoveRight(float Value)
{
	if (Controller != nullptr && Value != 0.f)
	{
		// Get the right movement vector based on controller's rotation
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
		const FRotationMatrix YawRotator(YawRotation);
		const FVector ControlForward = YawRotator.GetUnitAxis(EAxis::Y);
		AddMovementInput(ControlForward, Value);
	}
}

void ARogueCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void ARogueCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void ARogueCharacter::UsePressed()
{
	AWeapon* Weapon = Cast<AWeapon>(OverlappingItem);
	if (Weapon)
	{
		Weapon->Equip(GetMesh(), FName("handslot_r"));
	}
}

void ARogueCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARogueCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ARogueCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &ARogueCharacter::Turn);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &ARogueCharacter::LookUp);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &ARogueCharacter::MoveRight);

	PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(FName("Use"), IE_Pressed, this, &ARogueCharacter::UsePressed);
}

