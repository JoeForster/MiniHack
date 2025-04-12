// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

class USoundBase;
class UBoxComponent;

/**
 * 
 */
UCLASS()
class MINIHACK_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	AWeapon();

    void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);
    void Equip(USceneComponent* InParent, FName InSocketName);
	void ClearIgnoreActors();

	FORCEINLINE UBoxComponent* GetHurtBox() const { return HurtBox; }

protected:
	virtual void BeginPlay() override;

	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	
	UFUNCTION()
	void OnHurtBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


private:
	UPROPERTY(EditAnywhere, Category = "SFX");
	USoundBase* EquipSound;
	
	UPROPERTY(VisibleAnywhere, Category = "Weapon Logic")
	UBoxComponent* HurtBox;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* HurtBoxTraceStart;
	
	UPROPERTY(VisibleAnywhere)
	USceneComponent* HurtBoxTraceEnd;

	TArray<AActor*> IgnoreActors;
};
