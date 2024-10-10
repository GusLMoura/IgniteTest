// Fill out your copyright notice in the Description page of Project Settings.

#include "PathfindCharacterAnimInstance.h"
#include "Character/PathfindCharacter.h"

void UPathfindCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	PathfindCharacter = Cast<APathfindCharacter>(TryGetPawnOwner());
}

void UPathfindCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (!PathfindCharacter)
	{
		PathfindCharacter = Cast<APathfindCharacter>(TryGetPawnOwner());
	}

	if (!PathfindCharacter) return;

	if (!PathfindCharacter->GetReachedDestination())
	{
		bIsWalking = true;
	}
	else
	{
		bIsWalking = false;
	}
}
