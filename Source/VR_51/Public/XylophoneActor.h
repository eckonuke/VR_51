// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XylophoneActor.generated.h"

UCLASS()
class VR_51_API AXylophoneActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AXylophoneActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = BaseSettings)
		class USceneComponent* rootComp;

	UPROPERTY(EditAnywhere, Category = BaseSettings)
		TArray<class UChildActorComponent*> childComps;

	UPROPERTY(EditAnywhere, Category = BaseSettings)
		TSubclassOf<class AKeyboardActor> keyboard;

private:

};
