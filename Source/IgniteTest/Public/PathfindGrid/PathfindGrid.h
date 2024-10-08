// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PathfindGrid.generated.h"

enum class EPathfindBoxType : uint8;

UCLASS()
class IGNITETEST_API APathfindGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APathfindGrid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, Category = "Collider")
	class USceneComponent* BoxesParent;

	UPROPERTY(EditDefaultsOnly, Category = "Grid Properties")
	TSubclassOf<class APathfindBox> PathFindBoxClass;

	UPROPERTY(EditAnywhere, Category = "Grid Properties")
	int32 SizeX = 15;

	UPROPERTY(EditAnywhere, Category = "Grid Properties")
	int32 SizeY = 9;

	UPROPERTY(EditAnywhere, Category = "Grid Properties")
	FVector GridOrigin = FVector(0.f, 0.f, 0.f);

	UPROPERTY(EditAnywhere, Category = "Grid Properties")
	FRotator GridRotation = FRotator(50.f, 0.f, 0.f);;

	UPROPERTY(VisibleAnywhere, Category = "Character")
	class APathfindCharacter* Character;

	UPROPERTY(EditDefaultsOnly, Category = "Character")
	TSubclassOf<APathfindCharacter> CharacterClass;

	UPROPERTY(EditAnywhere, Category = "Grid Properties")
	FVector2D CharacterInitialBox = FVector2D(0.f, 4.f);

	void GeneratePathfindAndMoveCharacterToDestinationBox(APathfindBox* DestinationBox);

	APathfindBox* GetPlathfindBoxAtPosition(int32 PositionX, int32 PositionY);

	TArray<APathfindBox*> AStarFindPathToDestination(APathfindBox* StartBox, APathfindBox* DestinationBox);

private:
	TArray<TArray<APathfindBox*>> Boxes;
	TMap<EPathfindBoxType, float> TypesOfPathFindBoxPenalty;
	
	TArray<FVector2D> UnWalkableBoxes;
	TArray<FVector2D> SandBoxes;
	TArray<FVector2D> MudBoxes;

	void InitializePathfindBoxesPenaltyTMap();
	void InitializePathfindBoxesTypesLocation();
	void SetGridOriginAndInclination();
	void CreateGrid();
	void SetPathFindBoxType(APathfindBox* Box);
	void CreateCharacter();

	const int32 MOVE_STRAIGHT_CONST = 10;
	const int32 MOVE_DIAGONAL_CONST = 14;

	int32 CalculateHCost(APathfindBox* AnyBox, APathfindBox* DestinationBox);
	int32 CalculateGCost(APathfindBox* AnyBox, APathfindBox* DestinationBox);
	TArray<APathfindBox*> GetPathfindBoxNeighbours(APathfindBox* AnyBox);
	TArray<APathfindBox*> GetFinalPathToDestination(APathfindBox* DestinationBox);
};
