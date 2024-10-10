// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PathfindCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class IGNITETEST_API UPathfindCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly, Category = Character)
	class APathfindCharacter* PathfindCharacter;
};
