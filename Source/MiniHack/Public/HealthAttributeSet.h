#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "HealthAttributeSet.generated.h"

UCLASS()
class MINIHACK_API UHealthAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UHealthAttributeSet();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGameplayAttributeData CurrentHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGameplayAttributeData MaxHealth;

	ATTRIBUTE_ACCESSORS_BASIC(UHealthAttributeSet, CurrentHealth);
	ATTRIBUTE_ACCESSORS_BASIC(UHealthAttributeSet, MaxHealth);
};
