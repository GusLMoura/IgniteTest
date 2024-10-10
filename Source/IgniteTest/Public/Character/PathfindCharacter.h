// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PathfindCharacter.generated.h"

UCLASS()
class IGNITETEST_API APathfindCharacter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APathfindCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere, Category = "Movement")
	float Speed = 300.f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float CameraRotateSpeed = 10.f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float CameraRotationSensibility = 5.f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float CameraZoomSpeed = 2.f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float CameraZoomSensibility = 100.f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float CameraMinimunZoom = 100.f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float CameraMaximumZoom = 4000.f;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	float CameraDefaultZoom = 800.f;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveCharacterToPathfindBox(class APathfindBox* DestinationBox);
	void StartMoveCharacterThroughPath(TArray<APathfindBox*> Path);
	void VerifyPathIsEndedOrKeepMoving();
	void RotateCharacterToDestinationDirection(FVector DestinationPosition);
	FRotator CombineRotators(FRotator RotA, FRotator RotB);
	void RotateCameraHorizontal(float AxisValue);
	void RestoreCameraRotation();
	void Zoom(float AxisValue);
	void RestoreZoomToDefault();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* NewRootComponent;

	/** Skeletal Mesh */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* SkeletalMesh;

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	FVector CurrentLocation;
	FVector Destination;
	bool bReachedDestination = true;
	APathfindBox* CurrentLocatedPathfindBox;
	TArray<APathfindBox*> PathToMove;
	bool bIsMoving = false;
	int32 CurrentPathIndex = 0;

	FRotator CameraTargetRotation;
	FRotator CameraDefaultRotation;
	float CameraTargetZoom;

public:
	FORCEINLINE APathfindBox* GetCurrentLocatedPathfindBox() const { return CurrentLocatedPathfindBox; }
	FORCEINLINE void SetCurrentLocatedPathfindBox(APathfindBox* NewLocatedPathfindBox) { CurrentLocatedPathfindBox = NewLocatedPathfindBox; }
	FORCEINLINE bool GetReachedDestination() const { return bReachedDestination; }
};
