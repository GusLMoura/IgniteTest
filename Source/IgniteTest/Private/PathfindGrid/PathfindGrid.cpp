// Fill out your copyright notice in the Description page of Project Settings.


#include "PathfindGrid/PathfindGrid.h"
#include "Pathfind/PathfindBox.h"
#include "Character/PathfindCharacter.h"

// Sets default values
APathfindGrid::APathfindGrid()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxesParent = CreateDefaultSubobject<USceneComponent>(TEXT("Boxes Parent"));
	SetRootComponent(BoxesParent);
}

// Called when the game starts or when spawned
void APathfindGrid::BeginPlay()
{
	Super::BeginPlay();

	SetGridOriginAndInclination();

	CreateGrid();

	CreateCharacter();

	//MoveCharacterToBox();
}

void APathfindGrid::SetGridOriginAndInclination()
{
	if (BoxesParent)
	{
		BoxesParent->AddWorldOffset(GridOrigin);
		BoxesParent->AddWorldRotation(GridRotation);
	}
}

void APathfindGrid::CreateGrid()
{
	if (!PathFindBoxClass) return;

	//Create Grida
	for (int32 i = 0; i < SizeX; i++)
	{
		for (int32 j = 0; j < SizeY; j++)
		{
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			FVector Position;
			Position.X = (i * 200.f);
			Position.Y = (j * 200.f);
			APathfindBox* NewPathfindBox = GetWorld()->SpawnActor<APathfindBox>(PathFindBoxClass, Position, FRotator::ZeroRotator, SpawnParameters);
			if (BoxesParent && NewPathfindBox)
			{
				NewPathfindBox->AttachToComponent(BoxesParent, FAttachmentTransformRules::KeepRelativeTransform, "BoxesSocket");
				NewPathfindBox->SetGrid(this);
				NewPathfindBox->SetPosX(i);
				NewPathfindBox->SetPosY(j);
				Boxes.Add(NewPathfindBox);
			}
		}
	}
}

void APathfindGrid::CreateCharacter()
{
	if (CharacterClass)
	{
		APathfindBox* InitialPathfindBox = nullptr;
		
		for (auto Box : Boxes)
		{
			if (Box->GetPosX() == CharacterInitialBox.X && Box->GetPosY() == CharacterInitialBox.Y)
			{
				InitialPathfindBox = Box;
				break;
			}
		}

		if (InitialPathfindBox)
		{
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			Character = GetWorld()->SpawnActor<APathfindCharacter>(CharacterClass, InitialPathfindBox->GetActorLocation(), InitialPathfindBox->GetActorRotation(), SpawnParameters);
			if (Character)
			{
				APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
				if (PlayerController)
				{
					PlayerController->Possess(Character);
				}
			}
		}
	}
}

void APathfindGrid::MoveCharacterToBoxPosition(FVector BoxPosition)
{
	if (Character)
	{
		Character->MoveCharacterToPosition(BoxPosition);
	}
}

