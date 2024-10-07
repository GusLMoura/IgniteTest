// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PathfindGrid.generated.h"

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
	int32 SizeX = 20;

	UPROPERTY(EditAnywhere, Category = "Grid Properties")
	int32 SizeY = 10;

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

	void MoveCharacterToBoxPosition(FVector BoxPosition);

private:
	TArray<APathfindBox*> Boxes;
	void SetGridOriginAndInclination();
	void CreateGrid();
	void CreateCharacter();
};
