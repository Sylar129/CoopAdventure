#include "Transporter.h"

UTransporter::UTransporter()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTransporter::BeginPlay()
{
	Super::BeginPlay();
}

void UTransporter::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
