// Fill out your copyright notice in the Description page of Project Settings.


#include "PathfindGrid/PathfindGrid.h"
#include "Pathfind/PathfindBox.h"
#include "Character/PathfindCharacter.h"
#include "Algo/Reverse.h"

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

	InitializePathfindBoxesPenaltyTMap();

	InitializePathfindBoxesTypesLocation();

	SetGridOriginAndInclination();

	CreateGrid();

	CreateCharacter();
}

void APathfindGrid::InitializePathfindBoxesPenaltyTMap()
{
	TypesOfPathFindBoxPenalty.Add(EPathfindBoxType::EPBT_Grass, 1.f);
	TypesOfPathFindBoxPenalty.Add(EPathfindBoxType::EPBT_Sand, 1.5f);
	TypesOfPathFindBoxPenalty.Add(EPathfindBoxType::EPBT_Mud, 2.f);
}

void APathfindGrid::InitializePathfindBoxesTypesLocation()
{
	UnWalkableBoxes.Add(FVector2D(0,0));
	UnWalkableBoxes.Add(FVector2D(0,1));
	UnWalkableBoxes.Add(FVector2D(1,1));
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

	Boxes.SetNum(SizeX);

	for (TArray<APathfindBox*>& Row : Boxes)
	{
		Row.SetNum(SizeY);
	}

	//Create Grid
	for (int32 i = 0; i < SizeX; i++)
	{
		for (int32 j = 0; j < SizeY; j++)
		{
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			FVector Position;
			Position.X = (i * 201.f);
			Position.Y = (j * 201.f);
			APathfindBox* NewPathfindBox = GetWorld()->SpawnActor<APathfindBox>(PathFindBoxClass, Position, FRotator::ZeroRotator, SpawnParameters);
			if (BoxesParent && NewPathfindBox)
			{
				NewPathfindBox->AttachToComponent(BoxesParent, FAttachmentTransformRules::KeepRelativeTransform, "BoxesSocket");
				NewPathfindBox->SetGrid(this);
				NewPathfindBox->SetPosX(i);
				NewPathfindBox->SetPosY(j);
				Boxes[i][j] = NewPathfindBox;
				SetPathFindBoxType(NewPathfindBox);
				NewPathfindBox->SetBoxMaterialAccordingToType();
			}
		}
	}
}

void APathfindGrid::SetPathFindBoxType(APathfindBox* Box)
{
	for (auto Element : UnWalkableBoxes)
	{
		if (Box->GetPosX() == (int32)Element.X && Box->GetPosY() == (int32)Element.Y)
		{
			Box->SetPathfindBoxType(EPathfindBoxType::EPBT_NotWalkable);
			return;
		}
	}

	for (auto Element : SandBoxes)
	{
		if (Box->GetPosX() == (int32)Element.X && Box->GetPosY() == (int32)Element.Y)
		{
			Box->SetPathfindBoxType(EPathfindBoxType::EPBT_Sand);
			return;
		}
	}

	for (auto Element : MudBoxes)
	{
		if (Box->GetPosX() == (int32)Element.X && Box->GetPosY() == (int32)Element.Y)
		{
			Box->SetPathfindBoxType(EPathfindBoxType::EPBT_Mud);
			return;
		}
	}

	Box->SetPathfindBoxType(EPathfindBoxType::EPBT_Grass);
}

APathfindBox* APathfindGrid::GetPlathfindBoxAtPosition(int32 PositionX, int32 PositionY)
{
	return Boxes[PositionX][PositionY];
}

void APathfindGrid::CreateCharacter()
{
	if (CharacterClass)
	{
		APathfindBox* InitialPathfindBox = nullptr;

		if (CharacterInitialBox.X < SizeX && CharacterInitialBox.Y < SizeY)
		{
			InitialPathfindBox = Boxes[CharacterInitialBox.X][CharacterInitialBox.Y];
		}

		if (InitialPathfindBox)
		{
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			Character = GetWorld()->SpawnActor<APathfindCharacter>(CharacterClass, InitialPathfindBox->GetActorLocation(), InitialPathfindBox->GetActorRotation(), SpawnParameters);
			if (Character)
			{
				Character->SetCurrentLocatedPathfindBox(InitialPathfindBox);
				APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
				if (PlayerController)
				{
					PlayerController->Possess(Character);
				}
			}
		}
	}
}

TArray<APathfindBox*> APathfindGrid::AStarFindPathToDestination(APathfindBox* StartBox, APathfindBox* DestinationBox)
{
	TArray<APathfindBox*> NotVerifiedBoxesArray;
	TArray<APathfindBox*> VerifiedBoxesArray;

	NotVerifiedBoxesArray.Add(StartBox);

	//Reset all Pathfind Map
	for (int32 i = 0; i < SizeX; i++)
	{
		for (int32 j = 0; j < SizeY; j++)
		{
			APathfindBox* Box = Boxes[i][j];
			Box->SetGCost(MAX_int32);
			Box->SetFCost(MAX_int32);
			Box->SetPreviousPathFindBox(nullptr);
		}
	}

	//Set Origin Box Values
	StartBox->SetGCost(0);
	StartBox->SetHCost(CalculateHCost(StartBox, DestinationBox));
	StartBox->CalculateFCost();

	while (NotVerifiedBoxesArray.Num() > 0)
	{
		APathfindBox* CurrentBox = nullptr;
		for (int i = 0; i < NotVerifiedBoxesArray.Num(); i++)
		{
			if (i == 0)
			{
				CurrentBox = NotVerifiedBoxesArray[i];
			}
			else
			{
				if (NotVerifiedBoxesArray[i]->GetFCost() < CurrentBox->GetFCost())
				{
					CurrentBox = NotVerifiedBoxesArray[i];
				}
			}
		}

		if (CurrentBox && CurrentBox == DestinationBox)
		{
			//Reached our Goal
			return GetFinalPathToDestination(DestinationBox);
		}

		NotVerifiedBoxesArray.Remove(CurrentBox);
		VerifiedBoxesArray.Add(CurrentBox);

		//Verify Neighbours
		for (auto Neighbour : GetPathfindBoxNeighbours(CurrentBox))
		{
			//Ignore Verified and Not Walkable neighbours
			if (VerifiedBoxesArray.Contains(Neighbour)) continue;
			if (Neighbour->GetPathfindBoxType() == EPathfindBoxType::EPBT_NotWalkable)
			{
				VerifiedBoxesArray.Add(Neighbour);
				continue;
			}

			int32 CompareGCost = CurrentBox->GetGCost() + CalculateGCost(CurrentBox, Neighbour);
			
			if (CompareGCost < Neighbour->GetGCost())
			{
				Neighbour->SetPreviousPathFindBox(CurrentBox);
				Neighbour->SetGCost(CompareGCost);
				Neighbour->SetHCost(CalculateHCost(Neighbour, DestinationBox));
				Neighbour->CalculateFCost();

				if (!NotVerifiedBoxesArray.Contains(Neighbour))
				{
					NotVerifiedBoxesArray.Add(Neighbour);
				}
			}
		}
	}

	//We could not Find a Path to Destination
	return TArray<APathfindBox*>();
}

int32 APathfindGrid::CalculateHCost(APathfindBox* AnyBox, APathfindBox* DestinationBox)
{
	int32 XDistance = FMath::Abs(AnyBox->GetPosX() - DestinationBox->GetPosX());
	int32 YDistance = FMath::Abs(AnyBox->GetPosY() - DestinationBox->GetPosY());
	int32 Remaining = FMath::Abs(XDistance - YDistance);

	return MOVE_DIAGONAL_CONST * FMath::Min(XDistance, YDistance) + MOVE_STRAIGHT_CONST * Remaining;
}

int32 APathfindGrid::CalculateGCost(APathfindBox* AnyBox, APathfindBox* DestinationBox)
{
	//float PathFindBoxTerrainPenalty = *TypesOfPathFindBoxPenalty.Find(DestinationBox->GetPathfindBoxType());
	float PathFindBoxTerrainPenalty = 1.0f;

	int32 XDistance = FMath::Abs(AnyBox->GetPosX() - DestinationBox->GetPosX());
	int32 YDistance = FMath::Abs(AnyBox->GetPosY() - DestinationBox->GetPosY());
	int32 Remaining = FMath::Abs(XDistance - YDistance);

	return (MOVE_DIAGONAL_CONST * FMath::Min(XDistance, YDistance) + MOVE_STRAIGHT_CONST * Remaining) * PathFindBoxTerrainPenalty;
}

TArray<APathfindBox*> APathfindGrid::GetPathfindBoxNeighbours(APathfindBox* AnyBox)
{
	TArray<APathfindBox*> Neigbours;

	if (AnyBox->GetPosX() - 1 >= 0)
	{
		//Get Left Neighbour
		Neigbours.Add(GetPlathfindBoxAtPosition(AnyBox->GetPosX() - 1, AnyBox->GetPosY()));

		//Get Left Down Neighbour
		if(AnyBox->GetPosY() - 1 >= 0){ Neigbours.Add(GetPlathfindBoxAtPosition(AnyBox->GetPosX() - 1, AnyBox->GetPosY() - 1)); }

		//Get Left Up Neighbour
		if (AnyBox->GetPosY() + 1 < SizeY) { Neigbours.Add(GetPlathfindBoxAtPosition(AnyBox->GetPosX() - 1, AnyBox->GetPosY() + 1)); }
	}

	if (AnyBox->GetPosX() + 1 < SizeX)
	{
		//Get Right Neighbour
		Neigbours.Add(GetPlathfindBoxAtPosition(AnyBox->GetPosX() + 1, AnyBox->GetPosY()));

		//Get Right Down Neighbour
		if (AnyBox->GetPosY() - 1 >= 0) { Neigbours.Add(GetPlathfindBoxAtPosition(AnyBox->GetPosX() + 1, AnyBox->GetPosY() - 1)); }

		//Get Right Up Neighbour
		if (AnyBox->GetPosY() + 1 < SizeY) { Neigbours.Add(GetPlathfindBoxAtPosition(AnyBox->GetPosX() + 1, AnyBox->GetPosY() + 1)); }
	}

	//Get Down Neighbour 
	if(AnyBox->GetPosY() - 1 >= 0){ Neigbours.Add(GetPlathfindBoxAtPosition(AnyBox->GetPosX(), AnyBox->GetPosY() - 1)); }

	//Get Up Neighbour 
	if (AnyBox->GetPosY() + 1 < SizeY) { Neigbours.Add(GetPlathfindBoxAtPosition(AnyBox->GetPosX(), AnyBox->GetPosY() + 1)); }

	return Neigbours;
}

TArray<APathfindBox*> APathfindGrid::GetFinalPathToDestination(APathfindBox* DestinationBox)
{
	TArray<APathfindBox*> Path;
	Path.Add(DestinationBox);

	APathfindBox* CurrentNode = DestinationBox;

	while (CurrentNode->GetPreviousPathFindBox() != nullptr)
	{
		Path.Add(CurrentNode->GetPreviousPathFindBox());
		CurrentNode = CurrentNode->GetPreviousPathFindBox();
	}

	Algo::Reverse(Path);

	return Path;
}

void APathfindGrid::GeneratePathfindAndMoveCharacterToDestinationBox(APathfindBox* DestinationBox)
{
	if (Character)
	{
		Character->StartMoveCharacterThroughPath(AStarFindPathToDestination(Character->GetCurrentLocatedPathfindBox(), DestinationBox));
	}
}



