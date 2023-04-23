// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "Interfaces/HitInterface.h"
#include "Characters/Enemies/EnemyMovementStates.h"
#include "EnemyBase.generated.h"

class UCapsuleComponent;
class UBoxComponent;
class UCharacterMovementComponent;
class UPaperFlipbook;
class UPaperFlipbookComponent;
class UAttributeComponent;
class UHealthBarComponent;
class UNiagaraSystem;

UCLASS()
class LITTLEFOREST_API AEnemyBase : public ACharacterBase, public IHitInterface
{
	GENERATED_BODY()

public:

	AEnemyBase();

	virtual void Tick(float DeltaTime) override;
	virtual void Jump() override;
	virtual void GetHit_Implementation();
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintNativeEvent)
	void Attack();
	virtual void Attack_Implementation();
	UFUNCTION(BlueprintCallable)
	void AttackCooldown();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	EEnemyState EnemyState = EEnemyState::EES_Idle;

protected:

	virtual void BeginPlay() override;

	// Functions
	void SetFlipbook(EEnemyState State);

	// Will be connect to DieFadingVFX
	UFUNCTION(BlueprintNativeEvent)
	void Die();
	virtual void Die_Implementation();
	UFUNCTION(BlueprintCallable)
	void DeleteActor();

	// Components Variable
	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* EnemyCapsule;
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* EnemyHitBox;

	// For Code Variables
	UCharacterMovementComponent* EnemyMovement;
	UPaperFlipbookComponent* EnemySprite;

	// Flipbook Variables
	UPROPERTY(EditAnywhere, Category = "Flipbook")
	UPaperFlipbook* FB_Idle;
	UPROPERTY(EditAnywhere, Category = "Flipbook")
	UPaperFlipbook* FB_Walk;
	UPROPERTY(EditAnywhere, Category = "Flipbook")
	UPaperFlipbook* FB_Attack;
	UPROPERTY(EditAnywhere, Category = "Flipbook")
	UPaperFlipbook* FB_Dead;

	// VFX Materials
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprite Material")
	UMaterialInterface* DefaultMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprite Material")
	UMaterialInterface* TranslucentMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	UNiagaraSystem* DeadVFX;


private:

	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attribute;
	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarComponent;

	UPROPERTY(VisibleAnywhere)
	class AEnemyAIController* EnemyAIController;
};
