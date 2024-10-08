// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PathfindCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "DrawDebugHelpers.h"
#include "Pathfind/PathfindBox.h"

// Sets default values
APathfindCharacter::APathfindCharacter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//Create Mesh
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

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
	
}

// Called every frame
void APathfindCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bReachedDestination)
	{
		CurrentLocation = this->GetActorLocation();
		if (CurrentLocation != Destination)
		{
			FVector DestinationVector = Destination - CurrentLocation;
			FVector NewLocation;
			if (DestinationVector.Length() <= 1.f)
			{
				NewLocation = Destination;
			}
			else
			{
				NewLocation = CurrentLocation + DestinationVector.GetSafeNormal() * Speed * DeltaTime;
			}
			 
			SetActorLocation(NewLocation);
		}
		else
		{
			bReachedDestination = true;
		}
	}
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
		//DrawDebugSphere(GetWorld(), DestinationBox->GetActorLocation(), 25.f, 12, FColor::Blue, false, 1.f);
		Destination = DestinationBox->GetActorLocation();
		bReachedDestination = false;
		CurrentLocatedPathfindBox = DestinationBox;
	}
}

