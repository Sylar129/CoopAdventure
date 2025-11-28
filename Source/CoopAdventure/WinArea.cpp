#include "WinArea.h"

#include "CoopAdventureCharacter.h"

AWinArea::AWinArea()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	WinAreaBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WinAreaBox"));
	SetRootComponent(WinAreaBox);

	WinCondition = false;
}

void AWinArea::BeginPlay()
{
	Super::BeginPlay();
}

void AWinArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		if (!WinCondition)
		{
			TArray<AActor*> OverlappingActors;
			WinAreaBox->GetOverlappingActors(OverlappingActors, ACoopAdventureCharacter::StaticClass());

			if (OverlappingActors.Num() == 2)
			{
				WinCondition = true;
				UE_LOG(LogTemp, Display, TEXT("Win!"));
				MulticastRPCWin();
			}
		}
	}
}

void AWinArea::MulticastRPCWin_Implementation()
{
	OnWinCondition.Broadcast();
}
