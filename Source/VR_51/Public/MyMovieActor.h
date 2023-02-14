// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyMovieActor.generated.h"

UCLASS()
class VR_51_API AMyMovieActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyMovieActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class USceneComponent* rootComp;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* screenComp;
	UPROPERTY(EditAnywhere)
	TArray<class UMediaSource*> videoSource;
	UPROPERTY(EditAnywhere)
	TArray<class UMediaPlayer*> mediaPlayer;
	UPROPERTY(EditAnywhere)
	class UMediaSoundComponent* soundComp;
	UPROPERTY(EditAnywhere)
	class UWidgetComponent* playBack_Widget;

	void PlayMovie();
	void StopMovie();
	void ReverseMovie(float second);
	void ForwardMovie(float second);
	void ChangeMovie();

private:
	int32 videoNum = 0;
};
