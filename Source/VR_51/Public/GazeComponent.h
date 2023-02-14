// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GazeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VR_51_API UGazeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGazeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	FVector min = FVector(0.05f);
	UPROPERTY(EditAnywhere)
	FVector max = FVector(0.5f);
	UPROPERTY(EditAnywhere)
	bool increase = false;
	UPROPERTY(EditAnywhere)
	class AMyMovieActor* moviePlayer;

private:
	//UPROPERTY(EditAnywhere, meta = AllowAccessPrivate) private를 UPROPERTY로 접근 해야한다면 meta가 꼭 필요하다
	class AVR_Player* player;
	float currentSize;
	bool canChange = true;
	void ChangeSize(FVector minSize, FVector maxSize);
};
