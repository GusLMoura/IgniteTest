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

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* DebugDestinationPathfindBox;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* EnableCameraMovement;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* CameraMove;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* ChangeZoom;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* RestoreZoom;

protected:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

private:
	class APathfindCharacter* PathfindCharacter;
	bool bCameraCanRotate;

	void OnLeftMouseButtonClicked();
	void OnRightMouseButtonClicked();
	void OnEnableCameraMovementButtonPressed();
	void OnEnableCameraMovementButtonReleased();
	void OnThumbMouseButtonHolded(const struct FInputActionValue& Value);
	void OnMouseScroll(const struct FInputActionValue& Value);
	void OnButtonRestoreZoomPressed();
};
