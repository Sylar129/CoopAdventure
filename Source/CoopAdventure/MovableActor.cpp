#include "MovableActor.h"

AMovableActor::AMovableActor()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);

	Point1 = CreateDefaultSubobject<UArrowComponent>(TEXT("Point1"));
	Point1->SetupAttachment(RootComp);
	Point1->SetRelativeLocation({ 0, 0, 0 });

	Point2 = CreateDefaultSubobject<UArrowComponent>(TEXT("Point2"));
	Point2->SetupAttachment(RootComp);
	Point2->SetRelativeLocation({ 0, 0, 300 });

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComp);
	Mesh->SetIsReplicated(true);

	Transporter = CreateDefaultSubobject<UTransporter>(TEXT("Transporter"));
}

void AMovableActor::BeginPlay()
{
	Super::BeginPlay();

	Transporter->SetPoints(Point1->GetRelativeLocation() + GetActorLocation(), Point2->GetRelativeLocation() + GetActorLocation());
}

void AMovableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
