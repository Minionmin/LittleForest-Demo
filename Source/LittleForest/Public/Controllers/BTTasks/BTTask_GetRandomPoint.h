// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_GetRandomPoint.generated.h"

/**
 * 
 */
UCLASS()
class LITTLEFOREST_API UBTTask_GetRandomPoint : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	UBTTask_GetRandomPoint();

protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	
	UPROPERTY()
	APawn* EnemyPawn;
	UPROPERTY()
	class AEnemyAIController* EnemyAIController;

	FVector RandomLocation;
};
