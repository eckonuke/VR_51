// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MovieWidget.generated.h"

/**
 * 
 */
UCLASS()
class VR_51_API UMovieWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* btn_play;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* btn_stop;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* btn_reverse;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* btn_forward;

	class AMyMovieActor* movieActor;
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void Play();
	UFUNCTION()
	void Stop();
	UFUNCTION()
	void Reverse();
	UFUNCTION()
	void Forward();
};
