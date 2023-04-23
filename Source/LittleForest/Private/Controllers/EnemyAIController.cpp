// Fill out your copyright notice in the Description page of Project Settings.

#include "Controllers/EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "DrawDebugHelpers.h"

AEnemyAIController::AEnemyAIController()
{

}

void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	// Get Player Ref
	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	// Get Nav Ref
	NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
	
	if (AIBehavior)
	{
		RunBehaviorTree(AIBehavior);
		AIBlackBoard = GetBlackboardComponent();
		AIBlackBoard->SetValueAsVector(FName("StartLocation"), GetPawn()->GetActorLocation());
	}


}

void AEnemyAIController::StopTree(FString Reason)
{
	GetBrainComponent()->StopLogic(Reason);
}

bool AEnemyAIController::InTargetRange(AActor* Target, double Radius)
{
	const double DistanceToTarget = (Target->GetActorLocation() - GetPawn()->GetActorLocation()).Size();
	return DistanceToTarget <= Radius;
}