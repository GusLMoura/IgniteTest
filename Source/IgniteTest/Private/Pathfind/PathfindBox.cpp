// Fill out your copyright notice in the Description page of Project Settings.


#include "Pathfind/PathfindBox.h"
#include "Components/BoxComponent.h"
#include "PathfindGrid/PathfindGrid.h"

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

void APathfindBox::MoveCharacterToThisBox()
{
	if (MyGrid)
	{
		MyGrid->MoveCharacterToBoxPosition(this->GetActorLocation());
	}
}


