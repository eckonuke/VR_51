// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "IKAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class VR_51_API UIKAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector controllerLocation_Left;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector controllerLocation_Right;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator controllerRotation_Left;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator controllerRotation_Right;
};
