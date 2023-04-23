// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Potions/Potion.h"
#include "Characters/Players/PlayerBase.h"

void APotion::PickUp()
{
	if (ItemName != "")
	{
		GEngine->AddOnScreenDebugMessage(1, 4.f, FColor::Cyan, FString::Printf(TEXT("You got %s 1 ea"), *ItemName));
	}

	Destroy();
}

void APotion::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
	PlayerBase = Cast<APlayerBase>(OtherActor);
}

void APotion::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}
