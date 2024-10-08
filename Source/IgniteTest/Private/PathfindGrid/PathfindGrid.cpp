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
	TypesOfPathFindBoxPenalty.Add(EPathfindBoxType::EPBT_Mud, 5.f);
}

void APathfindGrid::InitializePathfindBoxesTypesLocation()
{
	//This function is hardcoded and is not safe we safe (we could overflow the grid limits by accident) 
	//TODO: Replaced this function for a solution on Unreal Editor 

	//Set Unwalkable Boxes Position on Grid
	UnWalkableBoxes.Add(FVector2D(0, 0));
	UnWalkableBoxes.Add(FVector2D(0, 1));
	UnWalkableBoxes.Add(FVector2D(0, 2));
	UnWalkableBoxes.Add(FVector2D(0, 3));
	UnWalkableBoxes.Add(FVector2D(0, 5));
	UnWalkableBoxes.Add(FVector2D(0, 6));
	UnWalkableBoxes.Add(FVector2D(0, 7));
	UnWalkableBoxes.Add(FVector2D(0, 8));
	UnWalkableBoxes.Add(FVector2D(1, 0));
	UnWalkableBoxes.Add(FVector2D(1, 1));
	UnWalkableBoxes.Add(FVector2D(1, 2));
	UnWalkableBoxes.Add(FVector2D(1, 3));
	UnWalkableBoxes.Add(FVector2D(1, 5));
	UnWalkableBoxes.Add(FVector2D(1, 6));
	UnWalkableBoxes.Add(FVector2D(1, 7));
	UnWalkableBoxes.Add(FVector2D(1, 8));
	UnWalkableBoxes.Add(FVector2D(3, 1));
	UnWalkableBoxes.Add(FVector2D(3, 2));
	UnWalkableBoxes.Add(FVector2D(3, 3));
	UnWalkableBoxes.Add(FVector2D(3, 4));
	UnWalkableBoxes.Add(FVector2D(3, 5));
	UnWalkableBoxes.Add(FVector2D(3, 6));
	UnWalkableBoxes.Add(FVector2D(3, 7));
	UnWalkableBoxes.Add(FVector2D(4, 4));
	UnWalkableBoxes.Add(FVector2D(5, 4));
	UnWalkableBoxes.Add(FVector2D(6, 4));
	UnWalkableBoxes.Add(FVector2D(7, 4));
	UnWalkableBoxes.Add(FVector2D(8, 4));
	UnWalkableBoxes.Add(FVector2D(9, 4));
	UnWalkableBoxes.Add(FVector2D(9, 5));
	UnWalkableBoxes.Add(FVector2D(9, 6));
	UnWalkableBoxes.Add(FVector2D(9, 7));
	UnWalkableBoxes.Add(FVector2D(9, 8));

	//Set Sand Boxes Position On Grid
	SandBoxes.Add(FVector2D(7, 0));
	SandBoxes.Add(FVector2D(7, 1));
	SandBoxes.Add(FVector2D(7, 7));
	SandBoxes.Add(FVector2D(8, 0));
	SandBoxes.Add(FVector2D(8, 1));
	SandBoxes.Add(FVector2D(8, 7));
	SandBoxes.Add(FVector2D(13, 0));
	SandBoxes.Add(FVector2D(13, 1));
	SandBoxes.Add(FVector2D(14, 0));
	SandBoxes.Add(FVector2D(14, 1));

	//Set Mud Boxes Position On Grid
	MudBoxes.Add(FVector2D(7, 2));
	MudBoxes.Add(FVector2D(7, 3));
	MudBoxes.Add(FVector2D(7, 5));
	MudBoxes.Add(FVector2D(7, 6));
	MudBoxes.Add(FVector2D(8, 2));
	MudBoxes.Add(FVector2D(8, 3));
	MudBoxes.Add(FVector2D(10, 4));
	MudBoxes.Add(FVector2D(10, 5));
	MudBoxes.Add(FVector2D(11, 4));
	MudBoxes.Add(FVector2D(11, 5));
	MudBoxes.Add(FVector2D(12, 4));
	MudBoxes.Add(FVector2D(12, 5));
	MudBoxes.Add(FVector2D(13, 4));
	MudBoxes.Add(FVector2D(13, 5));
	MudBoxes.Add(FVector2D(14, 4));
	MudBoxes.Add(FVector2D(14, 5));
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
	float PathFindBoxTerrainPenalty = *TypesOfPathFindBoxPenalty.Find(DestinationBox->GetPathfindBoxType());

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



