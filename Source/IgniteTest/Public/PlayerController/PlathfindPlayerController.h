// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlathfindPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class IGNITETEST_API APlathfindPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* SelectDestinationPathfindBox;

protected:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

private:
	void OnLeftMouseButtonClicked();
	
};
