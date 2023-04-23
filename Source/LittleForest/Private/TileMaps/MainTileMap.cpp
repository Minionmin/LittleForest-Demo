// Fill out your copyright notice in the Description page of Project Settings.


#include "TileMaps/MainTileMap.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"

AMainTileMap::AMainTileMap()
{
	LeftMapBoundary = CreateDefaultSubobject<UBoxComponent>("Left Map Boundary");
	LeftMapBoundary->SetupAttachment(GetRootComponent());
	LeftMapBoundary->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	LeftMapBoundary->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	LeftMapBoundary->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	LeftMapBoundary->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	LeftMapBoundary->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

	RightMapBoundary = CreateDefaultSubobject<UBoxComponent>("Right Map Boundary");
	RightMapBoundary->SetupAttachment(GetRootComponent());
	RightMapBoundary->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RightMapBoundary->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	RightMapBoundary->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	RightMapBoundary->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	RightMapBoundary->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
}

void AMainTileMap::BeginPlay()
{
	Super::BeginPlay();
}
