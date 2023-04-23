

#include "Platforms/OneWayPlatform.h"
#include "Components/BoxComponent.h"
#include "Characters/Players/PlayerBase.h"

// Sets default values
AOneWayPlatform::AOneWayPlatform()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	StaticMesh->SetupAttachment(GetRootComponent());
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//StaticMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	StaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	StaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	StaticMesh->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_Yes;
	StaticMesh->bHiddenInGame = true;

	Underneath = CreateDefaultSubobject<UBoxComponent>("Underneath Platform");
	Underneath->SetupAttachment(StaticMesh);
	Underneath->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Underneath->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	Underneath->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Underneath->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	Underneath->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

	Above = CreateDefaultSubobject<UBoxComponent>("Above Platform");
	Above->SetupAttachment(StaticMesh);
	Above->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Above->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	Above->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Above->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	Above->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

}

// Called when the game starts or when spawned
void AOneWayPlatform::BeginPlay()
{
	Super::BeginPlay();
	
	Underneath->OnComponentBeginOverlap.AddDynamic(this, &AOneWayPlatform::OnUnderneathOverlap);
	Underneath->OnComponentEndOverlap.AddDynamic(this, &AOneWayPlatform::OnUnderneathEndOverlap);

	Above->OnComponentBeginOverlap.AddDynamic(this, &AOneWayPlatform::OnAboveOverlap);
	Above->OnComponentEndOverlap.AddDynamic(this, &AOneWayPlatform::OnAboveEndOverlap);
}

void AOneWayPlatform::OnUnderneathOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PlayerBase = Cast<APlayerBase>(OtherActor);

	if (PlayerBase)
	{
		DisableCollision();
	}
}

void AOneWayPlatform::OnUnderneathEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	PlayerBase = Cast<APlayerBase>(OtherActor);

	if (PlayerBase)
	{
		EnableCollision();
	}
}

void AOneWayPlatform::OnAboveOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PlayerBase = Cast<APlayerBase>(OtherActor);

	if (PlayerBase)
	{
		PlayerBase->SetOverlappingPlatform(this);
	}
}

void AOneWayPlatform::OnAboveEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	PlayerBase = Cast<APlayerBase>(OtherActor);

	if (PlayerBase)
	{
		PlayerBase->SetOverlappingPlatform(nullptr);
	}
}

void AOneWayPlatform::DisableCollision()
{
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AOneWayPlatform::EnableCollision()
{
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

// Called every frame
void AOneWayPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

