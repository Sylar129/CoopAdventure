#include "Transporter.h"

#include "PressurePlate.h"

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

	for (AActor* TA : TriggerActors)
	{
		auto* PP = Cast<APressurePlate>(TA);
		if (PP)
		{
			PP->OnActivated.AddDynamic(this, &UTransporter::OnPressurePlateActivated);
			PP->OnDeactivated.AddDynamic(this, &UTransporter::OnPressurePlateDeactivated);
		}
	}
}

void UTransporter::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!TriggerActors.IsEmpty())
	{
		AllTriggerActorsTriggered = ActivatedTriggerCount == TriggerActors.Num();
	}
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

void UTransporter::OnPressurePlateActivated()
{
	ActivatedTriggerCount++;

	FString Msg = FString::Printf(TEXT("Transporter Activated: %d"), ActivatedTriggerCount);
	GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::White, Msg);
}

void UTransporter::OnPressurePlateDeactivated()
{
	ActivatedTriggerCount--;
	FString Msg = FString::Printf(TEXT("Transporter Activated: %d"), ActivatedTriggerCount);
	GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::White, Msg);
}
