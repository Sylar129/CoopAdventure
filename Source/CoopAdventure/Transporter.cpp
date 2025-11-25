#include "Transporter.h"

UTransporter::UTransporter()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);

	MoveTime = 3;
	ActivatedTriggerCount = 0;

	ArePointSet = false;
	StartPoint = FVector::Zero();
	EndPoint = FVector::Zero();
}

void UTransporter::BeginPlay()
{
	Super::BeginPlay();
}

void UTransporter::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UTransporter::SetPoints(const FVector& Start, const FVector& End)
{
	if (Start.Equals(End))
	{
		return;
	}
	StartPoint = Start;
	EndPoint = End;
	ArePointSet = true;
}
