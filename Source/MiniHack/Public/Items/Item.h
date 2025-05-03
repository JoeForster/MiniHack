#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class USphereComponent;
class UNiagaraComponent;

enum class EItemState : uint8
{
	EIS_Hovering,
	EIS_Carried
};

UCLASS()
class MINIHACK_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters")
	float Amplitude = 1.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Sine Parameters")
	float RunningTime = 0.f;

	UFUNCTION(BlueprintPure)
	float TransformedSin();

	UFUNCTION(BlueprintPure)
	float TransformedCosin();

	template<typename T>
	T Avg(T first, T second);

	UFUNCTION()
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ItemMesh;

	EItemState ItemState = EItemState::EIS_Hovering;

	UPROPERTY(VisibleAnywhere);
	USphereComponent* InteractSphere;
	
	UPROPERTY(EditAnywhere);
	UNiagaraComponent* SparkleEffect;

private:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sine Parameters", meta = (AllowPrivateAccess = "true"))
	float TimeConstant = 5.f;

};

template<typename T>
inline T AItem::Avg(T first, T second)
{
	return (first + second) / 2;
}
