// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "InputActionValue.h"
#include "CharacterMovementStates.h"
#include "PlayerBase.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UCapsuleComponent;
class UBoxComponent;
class UCharacterMovementComponent;
class UPaperFlipbook;
class UPaperFlipbookComponent;
class USoundBase;
class AItem;
class AOneWayPlatform;

UCLASS()
class LITTLEFOREST_API APlayerBase : public ACharacterBase
{
	GENERATED_BODY()
	
public:

	APlayerBase();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Jump() override;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* AttackBox;
	UPROPERTY(VisibleAnywhere)
		UCapsuleComponent* PlayerCapsule;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleInstanceOnly)
	EMovementState MovementState = EMovementState::EMS_Idle;

	// Components Variables
	UPROPERTY(VisibleAnywhere)
		USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere)
		UCameraComponent* ViewCamera;

	// Input Variables
	UPROPERTY(EditAnywhere, Category = "Input")
		UInputMappingContext* PlayerContext;
	UPROPERTY(EditAnywhere, Category = "Input")
		UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, Category = "Input")
		UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, Category = "Input")
		UInputAction* EKeyAction;
	UPROPERTY(EditAnywhere, Category = "Input")
		UInputAction* AttackAction;
	UPROPERTY(EditAnywhere, Category = "Input")
		UInputAction* DropAction;

	// Flipbook Variables
	UPROPERTY(EditAnywhere, Category = "Flipbook")
		UPaperFlipbook* FB_Idle;
	UPROPERTY(EditAnywhere, Category = "Flipbook")
		UPaperFlipbook* FB_RunRight;
	UPROPERTY(EditAnywhere, Category = "Flipbook")
		UPaperFlipbook* FB_RunLeft;
	UPROPERTY(EditAnywhere, Category = "Flipbook")
		UPaperFlipbook* FB_Jump;
	UPROPERTY(EditAnywhere, Category = "Flipbook")
		UPaperFlipbook* FB_Attack1;
	UPROPERTY(EditAnywhere, Category = "Flipbook")
		UPaperFlipbook* FB_Attack2;
	UPROPERTY(EditAnywhere, Category = "Flipbook")
		UPaperFlipbook* FB_Attack3;
	
	// SFX Variables
	UPROPERTY(EditAnywhere, Category = "SFX")
	USoundBase* JumpSFX;

	// For Code Variables
	UCharacterMovementComponent* PlayerMovement;
	UPaperFlipbookComponent* PlayerSprite;

	UPROPERTY(VisibleInstanceOnly)
		AItem* OverlappingItem;
	UPROPERTY(VisibleInstanceOnly)
		AOneWayPlatform* OverlappingPlatform;

	// Functions
	void Move(const FInputActionValue& Value);
	void TurnRight();
	void TurnLeft();
	bool CanJump();
	void EKeyPressed(const FInputActionValue& Value);
	void Still();
	void Drop(const FInputActionValue& Value);
	void PlayAttack(const FInputActionValue& Value);
	void Attack();
	void AttackCooldown();
	UFUNCTION()
	void OnAttackBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void SetFlipbook(EMovementState State);

private:

	UPROPERTY(VisibleAnywhere)
		USceneComponent* AttackBoxTraceStart;
	UPROPERTY(VisibleAnywhere)
		USceneComponent* AttackBoxTraceEnd;

	int32 AttackStance = 1;
	UPROPERTY(EditAnywhere, Category = "Damage")
	int32 Damage = 50;

public:

	FORCEINLINE void SetOverlappingItem(AItem* TheItem) { OverlappingItem = TheItem; }
	FORCEINLINE void SetOverlappingPlatform(AOneWayPlatform* ThePlatform) { OverlappingPlatform = ThePlatform; }
};
