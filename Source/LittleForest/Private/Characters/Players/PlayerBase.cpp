// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Players/PlayerBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "PaperFlipbookComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Controllers/LF_Controller.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Items/Potions/Potion.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/HitInterface.h"
#include "Platforms/OneWayPlatform.h"

APlayerBase::APlayerBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// Camera Stick Setting
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->SetWorldRotation(FRotator(0.f, -90.f, 0.f));

	// Camera Setting
	ViewCamera = CreateDefaultSubobject<UCameraComponent>("ViewCamera");
	ViewCamera->SetupAttachment(CameraBoom);
	ViewCamera->ProjectionMode = ECameraProjectionMode::Orthographic;
	ViewCamera->OrthoWidth = 400;

	// Attack Box Setting
	AttackBox = CreateDefaultSubobject<UBoxComponent>("Attack Box");
	AttackBox->SetupAttachment(GetRootComponent());
	AttackBox->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	AttackBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AttackBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	AttackBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);

	// Attack Box Tracing
	AttackBoxTraceStart = CreateDefaultSubobject<USceneComponent>("Attack Box Trace Start");
	AttackBoxTraceStart->SetupAttachment(GetRootComponent());
	AttackBoxTraceEnd = CreateDefaultSubobject<USceneComponent>("Attack Box Trace End");
	AttackBoxTraceEnd->SetupAttachment(GetRootComponent());

	// Player Collision Setting
	PlayerCapsule = GetCapsuleComponent();
	if (PlayerCapsule)
	{
		PlayerCapsule->SetCapsuleRadius(8.f);
		PlayerCapsule->SetCapsuleHalfHeight(15.f);
		PlayerCapsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		PlayerCapsule->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
		PlayerCapsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		PlayerCapsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		PlayerCapsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
		PlayerCapsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
		PlayerCapsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	}

	// Player Sprite Setting
	PlayerSprite = APaperCharacter::GetSprite();
	if (PlayerSprite)
	{
		PlayerSprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		PlayerSprite->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
		PlayerSprite->SetCollisionResponseToAllChannels(ECR_Ignore);
	}

	// Player Movement Setting
	PlayerMovement = GetCharacterMovement();
	if (PlayerMovement)
	{
		PlayerMovement->SetWalkableFloorAngle(0.f);
		PlayerMovement->MaxStepHeight = 0.f;
		PlayerMovement->MaxWalkSpeed = 90.f;
		PlayerMovement->JumpZVelocity = 320.f;
	}
}

void APlayerBase::Tick(float DeltaTime)
{
}

void APlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerBase::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::None, this, &APlayerBase::Still);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &APlayerBase::Jump);
		EnhancedInputComponent->BindAction(EKeyAction, ETriggerEvent::Triggered, this, &APlayerBase::EKeyPressed);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &APlayerBase::PlayAttack);
		EnhancedInputComponent->BindAction(DropAction, ETriggerEvent::Triggered, this, &APlayerBase::Drop);
	}
}

void APlayerBase::BeginPlay()
{
	Super::BeginPlay();

	if (ALF_Controller* CharacterController = Cast<ALF_Controller>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(CharacterController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerContext, 0);
		}
	}

	AttackBox->OnComponentBeginOverlap.AddDynamic(this, &APlayerBase::OnAttackBoxOverlap);
}

void APlayerBase::Jump()
{
	if (CanJump())
	{
		Super::Jump();
		UGameplayStatics::PlaySoundAtLocation(this, JumpSFX, GetActorLocation());
		MovementState = EMovementState::EMS_Jump;
		SetFlipbook(MovementState);
	}
}

void APlayerBase::Move(const FInputActionValue& Value)
{
	const float MoveValue = Value.Get<float>();
	bool bIsFalling = PlayerMovement->IsFalling();

	if (GetController() && !bIsFalling && MovementState != EMovementState::EMS_Attacking)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FVector Forward = GetActorForwardVector();

		AddMovementInput(Forward, MoveValue);

		if (MoveValue > 0)
		{
			MovementState = EMovementState::EMS_RunRight;
		}
		else
		{
			MovementState = EMovementState::EMS_RunLeft;
		}

		SetFlipbook(MovementState);
	}
}

void APlayerBase::TurnRight()
{
	AttackBoxTraceStart->SetRelativeLocation(FVector(9.f, 0.f, 0.f));
	AttackBoxTraceEnd->SetRelativeLocation(FVector(33.f, 0.f, 0.f));
	PlayerSprite->SetRelativeLocation(FVector(2.f, 0.f, 0.f));
	PlayerSprite->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	AttackBox->SetRelativeLocation(FVector(21.f, 0.f, 0.f));
}

void APlayerBase::TurnLeft()
{
	AttackBoxTraceStart->SetRelativeLocation(FVector(-9.f, 0.f, 0.f));
	AttackBoxTraceEnd->SetRelativeLocation(FVector(-33.f, 0.f, 0.f));
	PlayerSprite->SetRelativeLocation(FVector(-2.f, 0.f, 0.f));
	PlayerSprite->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
	AttackBox->SetRelativeLocation(FVector(-21.f, 0.f, 0.f));
}

bool APlayerBase::CanJump()
{
	return MovementState != EMovementState::EMS_Attacking && PlayerMovement->IsFalling() != true;
}

void APlayerBase::EKeyPressed(const FInputActionValue& Value)
{
	if (APotion* TheItem = Cast<APotion>(OverlappingItem))
	{
		TheItem->PickUp();
	}
}

void APlayerBase::Still()
{
	if (PlayerMovement->IsFalling() != true && MovementState != EMovementState::EMS_Attacking)
	{
		MovementState = EMovementState::EMS_Idle;
		SetFlipbook(MovementState);
	}
}

void APlayerBase::Drop(const FInputActionValue& Value)
{
	if (PlayerMovement->IsFalling()) { return; }

	if (AOneWayPlatform* ThePlatform = Cast<AOneWayPlatform>(OverlappingPlatform))
	{
			ThePlatform->DisableCollision();
	}
}

void APlayerBase::PlayAttack(const FInputActionValue& Value)
{

	if (MovementState != EMovementState::EMS_Attacking)
	{
		Attack();
	}
}

void APlayerBase::Attack()
{
	MovementState = EMovementState::EMS_Attacking;
	SetFlipbook(EMovementState::EMS_Attacking);

	// Enable Attack Box Collision
	AttackBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	// Attack Cooldown
	FTimerHandle AttackCooldownTime;
	GetWorldTimerManager().SetTimer(AttackCooldownTime, this, &APlayerBase::AttackCooldown, PlayerSprite->GetFlipbookLength(), false);
}

void APlayerBase::AttackCooldown()
{
	AttackBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (PlayerMovement->IsFalling())
	{
		MovementState = EMovementState::EMS_Jump;
	}
	else
	{
		MovementState = EMovementState::EMS_Idle;
	}
	SetFlipbook(MovementState);
}

void APlayerBase::OnAttackBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FVector TraceStart = AttackBoxTraceStart->GetComponentLocation();
	const FVector TraceEnd = AttackBoxTraceEnd->GetComponentLocation();
	
	TArray<AActor*> IgnoredActors;
	IgnoredActors.AddUnique(this);

	FHitResult ImpactHit;

	UKismetSystemLibrary::BoxTraceSingle(
		this,
		TraceStart,
		TraceEnd,
		FVector(6.f, 10.f, 7.f),
		AttackBoxTraceStart->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		IgnoredActors,
		EDrawDebugTrace::None,
		ImpactHit,
		true
	);

	UGameplayStatics::ApplyDamage(
		ImpactHit.GetActor(),
		Damage,
		GetController(),
		this,
		UDamageType::StaticClass()
	);

	if (ImpactHit.GetActor())
	{
		IHitInterface* HitInterface = Cast<IHitInterface>(ImpactHit.GetActor());
		if (HitInterface)
		{
			HitInterface->Execute_GetHit(ImpactHit.GetActor());
		}
	}

}

void APlayerBase::SetFlipbook(EMovementState State)
{
	// Random for an Attack Flipbook
	if (State == EMovementState::EMS_Attacking)
	{
		AttackStance = FMath::RandRange(1, 3);
	}

	switch (State)
	{
	case EMovementState::EMS_Idle:
		PlayerSprite->SetFlipbook(FB_Idle);
		break;
	case EMovementState::EMS_RunRight:
		TurnRight();
		PlayerSprite->SetFlipbook(FB_RunRight);
		break;
	case EMovementState::EMS_RunLeft:
		TurnLeft();
		PlayerSprite->SetFlipbook(FB_RunLeft);
		break;
	case EMovementState::EMS_Jump:
		PlayerSprite->SetFlipbook(FB_Jump);
		break;
	case EMovementState::EMS_Attacking:
		switch (AttackStance)
		{

		case 1:
			PlayerSprite->SetFlipbook(FB_Attack1);
			break;
		case 2:
			PlayerSprite->SetFlipbook(FB_Attack2);
			break;
		case 3:
			PlayerSprite->SetFlipbook(FB_Attack3);
			break;
		}
		break;
	default:
		PlayerSprite->SetFlipbook(FB_Idle);
		break;
	}
}
