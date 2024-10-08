// Fill out your copyright notice in the Description page of Project Settings.


#include "Pathfind/PathfindBox.h"
#include "Components/BoxComponent.h"
#include "PathfindGrid/PathfindGrid.h"
#include "Character/PathfindCharacter.h"
#include "DrawDebugHelpers.h"

// Sets default values
APathfindBox::APathfindBox()
{
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	SetRootComponent(BoxCollider);
}

// Called when the game starts or when spawned
void APathfindBox::BeginPlay()
{
	Super::BeginPlay();
}

void APathfindBox::CalculateFCost()
{
	FCost = GCost + HCost;
}

void APathfindBox::MoveCharacterToThisBox()
{
	if (MyGrid)
	{
		MyGrid->MoveCharacterToPathfindBox(this);
	}
}

void APathfindBox::DebugPathfindToThisBox()
{
	if (MyGrid)
	{
		DrawDebugSphere(GetWorld(), this->GetActorLocation(), 25.f, 12, FColor::Blue, false, 1.f);

		TArray<APathfindBox*> Path = MyGrid->AStarFindPathToDestination(MyGrid->Character->GetCurrentLocatedPathfindBox(), this);
		if (Path.Num() > 0)
		{
			for (auto Element : Path)
			{
				UE_LOG(LogTemp, Warning, TEXT("Nome: %s"), *Element->GetFullName());
			}
		}
	}
}


