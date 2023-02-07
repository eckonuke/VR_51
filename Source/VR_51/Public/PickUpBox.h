// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUpActor.h"
#include "PickUpBox.generated.h"

/**
 * 
 */
UCLASS()
class VR_51_API APickUpBox : public APickUpActor
{
	GENERATED_BODY()
	
public:
	APickUpBox();

	UPROPERTY(EditAnywhere)
		class UBoxComponent* rootComp;
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* meshComp;
};
