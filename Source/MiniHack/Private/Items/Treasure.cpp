#include "Items/Treasure.h"

#include "Characters/RogueCharacter.h"
#include "Kismet/GameplayStatics.h"

void ATreasure::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARogueCharacter* OtherCharacter = Cast<ARogueCharacter>(OtherActor);
	if (OtherCharacter != nullptr)
	{
		if (PickUpSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, PickUpSound, GetActorLocation());
		}
		Destroy();
	}
}
