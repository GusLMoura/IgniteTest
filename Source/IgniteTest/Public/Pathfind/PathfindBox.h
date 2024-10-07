// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PathfindBox.generated.h"

UCLASS()
class IGNITETEST_API APathfindBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APathfindBox();

	void MoveCharacterToThisBox();

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
};
