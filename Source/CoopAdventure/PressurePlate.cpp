// Fill out your copyright notice in the Description page of Project Settings.

#include "PressurePlate.h"

// Sets default values
APressurePlate::APressurePlate()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);

	TriggerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TriggerMesh"));
	TriggerMesh->SetupAttachment(RootComp);
	TriggerMesh->SetIsReplicated(true);
	auto TriggerMeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Game/LevelPrototyping/Meshes/SM_Cylinder.SM_Cylinder"));
	if (TriggerMeshAsset.Succeeded())
	{
		TriggerMesh->SetStaticMesh(TriggerMeshAsset.Object);
		TriggerMesh->SetRelativeScale3D({ 3.3, 3.3, 0.2 });
		TriggerMesh->SetRelativeLocation({ 0, 0, 10 });
	}

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComp);
	Mesh->SetIsReplicated(true);
	auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Game/Stylized_Egypt/Meshes/building/SM_building_part_08.SM_building_part_08"));
	if (MeshAsset.Succeeded())
	{
		Mesh->SetStaticMesh(MeshAsset.Object);
		Mesh->SetRelativeScale3D({ 4, 4, 0.5 });
		Mesh->SetRelativeLocation({ 0, 0, 7.2 });
	}

	Transporter = CreateDefaultSubobject<UTransporter>(TEXT("Transporter"));
	Transporter->MoveTime = 0.25f;
	Transporter->OwnerIsTriggerActor = true;

	Activated = false;
}

// Called when the game starts or when spawned
void APressurePlate::BeginPlay()
{
	Super::BeginPlay();

	TriggerMesh->SetVisibility(false);
	TriggerMesh->SetCollisionProfileName(FName("OverlapAll"));

	Transporter->SetPoints(GetActorLocation(), GetActorLocation() - FVector(0, 0, 10));
}

// Called every frame
void APressurePlate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		TArray<AActor*> OverlappingActors;
		TriggerMesh->GetOverlappingActors(OverlappingActors);
		AActor* TriggerActor = nullptr;

		for (AActor* A : OverlappingActors)
		{
			if (A->ActorHasTag("TriggerActor"))
			{
				TriggerActor = A;
				break;
			}
			// FString Msg = FString::Printf(TEXT("Name: %s"), *A->GetName());
			// GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::White, Msg);
		}

		if (TriggerActor)
		{
			if (!Activated)
			{
				Activated = true;
				OnActivated.Broadcast();
			}
		}
		else
		{
			if (Activated)
			{
				Activated = false;
				OnDeactivated.Broadcast();
			}
		}
	}
}
