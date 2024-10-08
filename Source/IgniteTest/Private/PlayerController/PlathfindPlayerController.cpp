// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/PlathfindPlayerController.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Pathfind/PathfindBox.h"

void APlathfindPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (SelectDestinationPathfindBox)
		{
			EnhancedInputComponent->BindAction(SelectDestinationPathfindBox, ETriggerEvent::Triggered, this, &APlathfindPlayerController::OnLeftMouseButtonClicked);
		}

		if (DebugDestinationPathfindBox)
		{
			EnhancedInputComponent->BindAction(DebugDestinationPathfindBox, ETriggerEvent::Started, this, &APlathfindPlayerController::OnRightMouseButtonClicked);
		}
	}
}

void APlathfindPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void APlathfindPlayerController::OnLeftMouseButtonClicked()
{
	FVector WorldLocation;
	FVector WorldDirection;
	if (DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	{
		FHitResult HitResult;
		FCollisionQueryParams TraceParams;
		GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, WorldLocation + WorldDirection * 5000.f, ECC_Visibility, TraceParams);
		if (HitResult.GetActor())
		{
			APathfindBox* ClickedPathfindBox = Cast<APathfindBox>(HitResult.GetActor());
			//Dont Move to Not Walkable Boxes
			if (ClickedPathfindBox && ClickedPathfindBox->GetPathfindBoxType() != EPathfindBoxType::EPBT_NotWalkable)
			{
				ClickedPathfindBox->GeneratePathfindToThisBox();
			}
		}
	}
}

void APlathfindPlayerController::OnRightMouseButtonClicked()
{
	FVector WorldLocation;
	FVector WorldDirection;
	if (DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	{
		FHitResult HitResult;
		FCollisionQueryParams TraceParams;
		GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, WorldLocation + WorldDirection * 5000.f, ECC_Visibility, TraceParams);
		if (HitResult.GetActor())
		{
			APathfindBox* ClickedPathfindBox = Cast<APathfindBox>(HitResult.GetActor());
			if (ClickedPathfindBox && ClickedPathfindBox->GetPathfindBoxType() != EPathfindBoxType::EPBT_NotWalkable)
			{
				ClickedPathfindBox->DebugPathfindToThisBox();
			}
		}
	}
}
