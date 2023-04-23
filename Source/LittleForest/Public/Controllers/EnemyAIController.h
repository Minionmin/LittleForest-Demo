// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class LITTLEFOREST_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:

	AEnemyAIController();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void StopTree(FString Reason);
	UFUNCTION()
	bool InTargetRange(AActor* Target, double Radius);

	// Combat
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	AActor* CombatTarget;
	UPROPERTY(EditAnywhere, Category = "Combat")
	double CombatRadius = 100.f;

	// Patrol
	UPROPERTY(VisibleAnywhere, Category = "Patrol")
	class UNavigationSystemV1* NavArea;
	UPROPERTY(EditAnywhere, Category = "Patrol")
	double PatrolRadius = 100.f;
	UPROPERTY(VisibleAnywhere, Category = "Patrol")
	FVector RandomLocation;

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY()
	APawn* PlayerPawn;
	UPROPERTY()
	FVector PlayerLocation;
	UPROPERTY()
	FVector PlayerLastLocation;
	UPROPERTY()
	FVector EnemyStartLocation;

	UPROPERTY(EditAnywhere, Category = "AI")
	class UBehaviorTree* AIBehavior;
	class UBlackboardComponent* AIBlackBoard;
};
