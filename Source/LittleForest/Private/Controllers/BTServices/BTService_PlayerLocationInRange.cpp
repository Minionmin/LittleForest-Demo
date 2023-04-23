// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/BTServices/BTService_PlayerLocationInRange.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Controllers/EnemyAIController.h"

UBTService_PlayerLocationInRange::UBTService_PlayerLocationInRange()
{
	NodeName = TEXT("Update Player Location");
}

void UBTService_PlayerLocationInRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn == nullptr)
	{
		return;
	}


	AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if (EnemyAIController && EnemyAIController->InTargetRange(PlayerPawn, EnemyAIController->CombatRadius))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), PlayerPawn->GetActorLocation());
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
	}
}
