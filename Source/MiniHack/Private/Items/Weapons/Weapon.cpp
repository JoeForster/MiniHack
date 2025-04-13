// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"
#include "Characters/RogueCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Interfaces/HitReceiver.h"

AWeapon::AWeapon()
{
	HurtBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hurt Box"));
	HurtBox->SetupAttachment(RootComponent);
	HurtBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HurtBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	HurtBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	HurtBoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Hurt Box Trace Start"));
	HurtBoxTraceStart->SetupAttachment(GetRootComponent());

	HurtBoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Hurt Box Trace End"));
	HurtBoxTraceEnd->SetupAttachment(GetRootComponent());
}

void AWeapon::AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);
}

void AWeapon::Equip(USceneComponent* InParent, FName InSocketName)
{
	AttachMeshToSocket(InParent, InSocketName);
	ItemState = EItemState::EIS_Carried;
	if (EquipSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, EquipSound, GetActorLocation());
	}
	if (SphereCppTest)
	{
		SphereCppTest->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	if (HurtBox)
	{
		HurtBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AWeapon::ClearIgnoreActors()
{
	IgnoreActors.Empty();
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (HurtBox)
	{
		HurtBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnHurtBoxBeginOverlap);
	}
}

void AWeapon::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AWeapon::OnHurtBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FHitResult HitResult;

	const FVector Start = HurtBoxTraceStart->GetComponentLocation();
	const FVector End = HurtBoxTraceEnd->GetComponentLocation();
	const FVector HalfSize (5.f, 5.f, 5.f);
	const bool bTraceComplex = false;
	const bool bIgnoreSelf = true;

	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		HalfSize,
		HurtBoxTraceStart->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		bTraceComplex,
		IgnoreActors,
		EDrawDebugTrace::Type::None,
		HitResult,
		bIgnoreSelf);

	if (IHitReceiver* HitReceiver = Cast<IHitReceiver>(HitResult.GetActor()))
	{
		HitReceiver->ReceiveHit(HitResult.ImpactPoint);
		// Stop hitting already-hit actors until the swing finishes
		IgnoreActors.AddUnique(HitResult.GetActor());
	}
}
