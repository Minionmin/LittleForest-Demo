// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/BTTasks/BTTask_GetRandomPoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Controllers/EnemyAIController.h"
#include "NavigationSystem.h"

UBTTask_GetRandomPoint::UBTTask_GetRandomPoint()
{
	NodeName = TEXT("Get Random Point");
}

EBTNodeResult::Type UBTTask_GetRandomPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	// Get Ref for AIController and Enemy Pawn
	EnemyAIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	EnemyPawn = EnemyAIController->GetPawn();

	// Random Patrol Point
	if (EnemyAIController->NavArea)
	{
		EnemyAIController->NavArea->K2_GetRandomReachablePointInRadius(GetWorld(), EnemyPawn->GetActorLocation(), RandomLocation, EnemyAIController->PatrolRadius);
	}

	// Adjust RandomLocation so it is 2D
	RandomLocation = FVector(RandomLocation.X, EnemyPawn->GetActorLocation().Y, EnemyPawn->GetActorLocation().Z);

	// Set RandomPoint value
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), RandomLocation);

	return EBTNodeResult::Succeeded;
}
