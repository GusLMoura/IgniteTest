// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PathfindBox.generated.h"

UENUM(BlueprintType)
enum class EPathfindBoxType : uint8
{
	EPBT_Grass UMETA(DisplayName = "Grass"),
	EPBT_Sand UMETA(DisplayName = "Sand"),
	EPBT_Mud UMETA(DisplayName = "Mud"),
	EPBT_NotWalkable UMETA(DisplayName = "NotWalkable"),

	EPBT_MAX UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class IGNITETEST_API APathfindBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APathfindBox();

	void CalculateFCost();
	void MoveCharacterToThisBox();
	void DebugPathfindToThisBox();
	
private:
	UPROPERTY(VisibleAnywhere, Category = "Collider")
	class UBoxComponent* BoxCollider;

	class APathfindGrid* MyGrid;

	UPROPERTY(VisibleAnywhere, Category = "Pathfind")
	int32 PosX;

	UPROPERTY(VisibleAnywhere, Category = "Pathfind")
	int32 PosY;

	UPROPERTY(VisibleAnywhere, Category = "Pathfind")
	int32 GCost;

	UPROPERTY(VisibleAnywhere, Category = "Pathfind")
	int32 HCost;

	UPROPERTY(VisibleAnywhere, Category = "Pathfind")
	int32 FCost;

	UPROPERTY(VisibleAnywhere, Category = "Pathfind")
	EPathfindBoxType PathfindBoxType = EPathfindBoxType::EPBT_Grass;

	UPROPERTY(VisibleAnywhere, Category = "Pathfind")
	APathfindBox* PreviousPathFindBox;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	FORCEINLINE void SetGrid(APathfindGrid* Grid) { MyGrid = Grid; }
	FORCEINLINE int32 GetPosX() const { return PosX; }
	FORCEINLINE void SetPosX(int32 X) { PosX = X; }
	FORCEINLINE int32 GetPosY() const { return PosY; }
	FORCEINLINE void SetPosY(int32 Y) { PosY = Y; }
	FORCEINLINE int32 GetGCost() const { return GCost; }
	FORCEINLINE void SetGCost(int32 newGCost) { GCost = newGCost; }
	FORCEINLINE int32 GetHCost() const { return HCost; }
	FORCEINLINE void SetHCost(int32 newHCost) { HCost = newHCost; }
	FORCEINLINE int32 GetFCost() const { return FCost; }
	FORCEINLINE void SetFCost(int32 newFCost) { HCost = newFCost; }
	FORCEINLINE APathfindBox* GetPreviousPathFindBox() const { return PreviousPathFindBox; }
	FORCEINLINE void SetPreviousPathFindBox(APathfindBox* NewPreviousPathFindBox) { PreviousPathFindBox = NewPreviousPathFindBox; }
	FORCEINLINE EPathfindBoxType GetPathfindBoxType() const { return PathfindBoxType; }
};
