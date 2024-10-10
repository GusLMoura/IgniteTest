// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PathfindCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "DrawDebugHelpers.h"
#include "Pathfind/PathfindBox.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APathfindCharacter::APathfindCharacter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//Create Root Component
	NewRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("NewRoot"));
	SetRootComponent(NewRootComponent);

	//Create Mesh
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	SkeletalMesh->SetupAttachment(RootComponent);

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}

// Called when the game starts or when spawned
void APathfindCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Get Current Spring Arm Rotation for Target Rotation as we change it
	if (CameraBoom)
	{
		//const FRotator Rotation = CameraBoom->GetRelativeRotation();
		CameraTargetRotation = CameraBoom->GetRelativeRotation();
		CameraDefaultRotation = CameraTargetRotation;
	}
}

// Called every frame
void APathfindCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bReachedDestination)
	{
		CurrentLocation = this->GetActorLocation();
		bool RotationCheckReachedDestination = false;

		if (CurrentLocation != Destination)
		{
			
			FVector DestinationVector = Destination - CurrentLocation;
			FVector NewLocation;
			if (DestinationVector.Length() <= 2.f)
			{
				NewLocation = Destination;
				RotationCheckReachedDestination = true;
			}
			else
			{
				NewLocation = CurrentLocation + DestinationVector.GetSafeNormal() * Speed * DeltaTime;
			}
			 
			SetActorLocation(NewLocation);

			if(!RotationCheckReachedDestination) RotateCharacterToDestinationDirection(Destination);
		}
		else
		{
			bReachedDestination = true;
			RotationCheckReachedDestination = false;
			CurrentLocatedPathfindBox = PathToMove[CurrentPathIndex];
			VerifyPathIsEndedOrKeepMoving();
		}
	}

	const FRotator InterpolatedRotation = UKismetMathLibrary::RInterpTo(CameraBoom->GetRelativeRotation(), CameraTargetRotation, DeltaTime, CameraRotateSpeed);
	CameraBoom->SetRelativeRotation(InterpolatedRotation);
}

// Called to bind functionality to input
void APathfindCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APathfindCharacter::MoveCharacterToPathfindBox(APathfindBox* DestinationBox)
{
	if (DestinationBox != CurrentLocatedPathfindBox)
	{
		Destination = DestinationBox->GetActorLocation();
		bReachedDestination = false;
		
	}
}

void APathfindCharacter::StartMoveCharacterThroughPath(TArray<APathfindBox*> Path)
{
	if (!bIsMoving)
	{
		bIsMoving = true;
		PathToMove = Path;
		CurrentPathIndex = 0;
		Destination = PathToMove[0]->GetActorLocation();
		bReachedDestination = false;
	}
}

void APathfindCharacter::VerifyPathIsEndedOrKeepMoving()
{
	CurrentPathIndex++;
	if (PathToMove.IsValidIndex(CurrentPathIndex))
	{
		Destination = PathToMove[CurrentPathIndex]->GetActorLocation();
		bReachedDestination = false;
	}
	else
	{
		bIsMoving = false;
		PathToMove.Empty();
	}
}

void APathfindCharacter::RotateCharacterToDestinationDirection(FVector DestinationPosition)
{
	FRotator WorldLookAtRotation = (DestinationPosition - this->GetActorLocation()).Rotation();
	FRotator LocalLookAtRotation = CombineRotators(WorldLookAtRotation, this->GetActorRotation() * -1);

	SkeletalMesh->SetRelativeRotation(FRotator(SkeletalMesh->GetRelativeRotation().Pitch, LocalLookAtRotation.Yaw -90.f, SkeletalMesh->GetRelativeRotation().Roll));
}

FRotator APathfindCharacter::CombineRotators(FRotator RotA, FRotator RotB)
{
	FQuat AQuat = FQuat(RotA);
	FQuat BQuat = FQuat(RotB);

	return FRotator(BQuat * AQuat);
}

void APathfindCharacter::RotateCameraHorizontal(float AxisValue)
{
	if (AxisValue == 0.f) return;

	CameraTargetRotation = UKismetMathLibrary::ComposeRotators(CameraTargetRotation, FRotator(0.f, AxisValue * CameraSensibility, 0.f));
}

void APathfindCharacter::RestoreCameraRotation()
{
	CameraTargetRotation = CameraDefaultRotation;
}
