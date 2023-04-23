// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteActor.h"
#include "Item.generated.h"

class USphereComponent;
class UPaperFlipbookComponent;
class UNiagaraComponent;

UCLASS()
class LITTLEFOREST_API AItem : public APaperSpriteActor
{
	GENERATED_BODY()
	
public:

	AItem();
	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

	// For code Variables
	UPaperFlipbookComponent* ItemSprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sin Parameters")
	float Amplitude = 0.1f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sin Parameters")
	float TimeConstant = 3.f;
	UPROPERTY(EditAnywhere, Category = "Item Properties")
	FString ItemName;
	UPROPERTY(EditAnywhere, Category = "VFX")
	UNiagaraComponent* GlowParticlesEffect;

	UFUNCTION(BlueprintPure) // function doesn't set any new value
	float TransformedSin();
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float RunningTime = 0;

	UPROPERTY(VisibleAnywhere)
		USphereComponent* Sphere;

};
