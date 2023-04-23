// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/EnemyBase.h"
#include "DrawDebugHelpers.h"
#include "PaperFlipbookComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Materials/MaterialInterface.h"
#include "Components/AttributeComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "HUD/HealthBarComponent.h"
#include "Controllers/EnemyAIController.h"

AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// EnemySprite
	EnemySprite = GetSprite();
	if (EnemySprite)
	{
		EnemySprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		EnemySprite->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
		EnemySprite->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	}

	// Enemy Capsule
	EnemyCapsule = GetCapsuleComponent();
	if (EnemyCapsule)
	{
		EnemyCapsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		EnemyCapsule->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
		EnemyCapsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		EnemyCapsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
		EnemyCapsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
		EnemyCapsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
		EnemyCapsule->SetGenerateOverlapEvents(true);
		EnemyCapsule->SetCapsuleHalfHeight(16.f);
		EnemyCapsule->SetCapsuleRadius(12.f);
	}

	// Enemy HitBox
	EnemyHitBox = CreateDefaultSubobject<UBoxComponent>("Enemy HitBox");
	EnemyHitBox->SetupAttachment(GetRootComponent());
	if (EnemyHitBox)
	{
		EnemyHitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		EnemyHitBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		EnemyHitBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		EnemyHitBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
		EnemyHitBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		EnemyHitBox->SetGenerateOverlapEvents(true);
		EnemyHitBox->SetBoxExtent(FVector(10.f, 10.f, 12.f));
		EnemyHitBox->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	}

	// Enemy Movement
	EnemyMovement = GetCharacterMovement();
	if (EnemyMovement)
	{
		EnemyMovement->MaxWalkSpeed = 35.f;
	}

	// Attribute Component
	Attribute = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attribute"));

	// Health Bar Widget
	HealthBarComponent = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarComponent->SetupAttachment(GetRootComponent());

}

void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyBase::Jump()
{
	Super::Jump();
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	// Initializing health bar
	if (Attribute && HealthBarComponent)
	{
		HealthBarComponent->SetHealthPercent(Attribute->GetHealthPercent());
		HealthBarComponent->SetVisibility(false);
	}

	EnemyAIController = Cast<AEnemyAIController>(GetController());
}

void AEnemyBase::GetHit_Implementation()
{	
	// Will play GetHitVFX in Blueprint first
	if (Attribute && !Attribute->IsAlive())
	{
		Die();
	}
	else
	{
		if (HealthBarComponent)
		{
			HealthBarComponent->SetVisibility(true);
		}

		EnemyState = EEnemyState::EES_IsHit;
	}
}

float AEnemyBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Attribute && HealthBarComponent)
	{
		Attribute->ReceiveDamage(DamageAmount);
		HealthBarComponent->SetHealthPercent(Attribute->GetHealthPercent());
	}

	return DamageAmount;
}

void AEnemyBase::Attack_Implementation()
{
	if (EnemyState != EEnemyState::EES_IsDead)
	{
		EnemyState = EEnemyState::EES_IsAttacking;
		SetFlipbook(EnemyState);
	}
}

void AEnemyBase::AttackCooldown()
{
	if (Attribute->IsAlive())
	{
		EnemyState = EEnemyState::EES_Idle;
	}
	else
	{
		EnemyState = EEnemyState::EES_IsDead;
	}
	SetFlipbook(EnemyState);
}

void AEnemyBase::Die_Implementation()
{
	// Will play DieFadingVFX first
	EnemyState = EEnemyState::EES_IsDead;
	EnemyAIController->StopTree(TEXT("Dead"));
	EnemySprite->SetMaterial(0, TranslucentMaterial);
	SetFlipbook(EnemyState);
}

void AEnemyBase::DeleteActor()
{
	EnemySprite->SetMaterial(0, DefaultMaterial);
	EnemyCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HealthBarComponent->SetVisibility(false);
	Destroy();
}

void AEnemyBase::SetFlipbook(EEnemyState State)
{
	switch (State)
	{
	case EEnemyState::EES_Idle:
		EnemySprite->SetFlipbook(FB_Idle);
		break;
	case EEnemyState::EES_IsDead:
		EnemySprite->SetFlipbook(FB_Dead);
		break;
	case EEnemyState::EES_IsAttacking:
		EnemySprite->SetFlipbook(FB_Attack);
		break;
	default:
		EnemySprite->SetFlipbook(FB_Idle);
		break;
	}
}
