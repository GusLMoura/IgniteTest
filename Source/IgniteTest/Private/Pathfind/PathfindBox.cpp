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

	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxMesh"));
	BoxMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APathfindBox::BeginPlay()
{
	Super::BeginPlay();
}

void APathfindBox::SetBoxMaterialAccordingToType()
{
	if (BoxMesh)
	{
		UMaterial* MaterialToSet = nullptr;

		switch (PathfindBoxType)
		{
			case EPathfindBoxType::EPBT_Grass:
			{
				if (GrassBoxMaterial)
				{
					MaterialToSet = GrassBoxMaterial;
				}
			} break;
			
			case EPathfindBoxType::EPBT_Sand:
			{
				if (SandBoxMaterial)
				{
					MaterialToSet = SandBoxMaterial;
				}
			} break;

			case EPathfindBoxType::EPBT_Mud:
			{
				if (MudBoxMaterial)
				{
					MaterialToSet = MudBoxMaterial;
				}
			} break;

			case EPathfindBoxType::EPBT_NotWalkable:
			{
				if (UnwalkableBoxMaterial)
				{
					MaterialToSet = UnwalkableBoxMaterial;
				}
			} break;
		}

		if (MaterialToSet)
		{
			BoxMesh->SetMaterial(0, MaterialToSet);
		}
	}
}

void APathfindBox::CalculateFCost()
{
	FCost = GCost + HCost;
}

void APathfindBox::GeneratePathfindToThisBox()
{
	if (MyGrid)
	{
		MyGrid->GeneratePathfindAndMoveCharacterToDestinationBox(this);
	}
}

void APathfindBox::DebugPathfindToThisBox()
{
	if (MyGrid)
	{
		

		TArray<APathfindBox*> Path = MyGrid->AStarFindPathToDestination(MyGrid->Character->GetCurrentLocatedPathfindBox(), this);
		if (Path.Num() > 0)
		{
			for (int i = 0; i < Path.Num() - 1; i++)
			{
				DrawDebugSphere(GetWorld(), this->GetActorLocation(), 25.f, 12, FColor::Blue, false, 3.f);
				DrawDebugLine(GetWorld(), Path[i]->GetActorLocation(), Path[i + 1]->GetActorLocation(), FColor::Blue, false, 3.f, 12, 10.f);
			}
		}
	}
}


