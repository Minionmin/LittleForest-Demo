// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperTileMapActor.h"
#include "MainTileMap.generated.h"

class UBoxComponent;
class APlayerBase;

UCLASS()
class LITTLEFOREST_API AMainTileMap : public APaperTileMapActor
{
	GENERATED_BODY()

public:
	
	AMainTileMap();
	
	virtual void BeginPlay() override;
	APlayerBase* PlayerBase;

private:

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* LeftMapBoundary;
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* RightMapBoundary;
};
