// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMovieActor.h"
#include <Components/StaticMeshComponent.h>
#include "Components/SceneComponent.h"
#include <MediaAssets/Public/MediaSource.h>
#include <MediaAssets/Public/MediaPlayer.h>
#include <MediaAssets/Public/MediaSoundComponent.h>
#include <UMG/Public/Components/WidgetComponent.h>
#include "MovieWidget.h"

// Sets default values
AMyMovieActor::AMyMovieActor() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(rootComp);

	screenComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Screen"));
	screenComp->SetupAttachment(RootComponent);

	soundComp = CreateDefaultSubobject<UMediaSoundComponent>(TEXT("Sound"));
	soundComp->SetupAttachment(RootComponent);

	playBack_Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget Component"));
	playBack_Widget->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMyMovieActor::BeginPlay() {
	Super::BeginPlay();
	if (playBack_Widget != nullptr) {
		UMovieWidget* movieWidget = Cast<UMovieWidget>(playBack_Widget->GetWidget());
		if (movieWidget != nullptr) {
			movieWidget->movieActor = this;
		}
	}
}

// Called every frame
void AMyMovieActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

void AMyMovieActor::PlayMovie() {
	//UE_LOG(LogTemp, Warning, TEXT("Pressed PlayMovie Function!!!!!!!!!!!!!!!!!!!!"));
	//미디어 플레이 테스트
	for (int32 i = 0; i < videoSource.Num(); i++) {
		if (videoSource[i] != nullptr && mediaPlayer[0] != nullptr) {
			if (mediaPlayer[i]->IsPlaying()) {
				mediaPlayer[i]->Pause();
			}
			else {
				if (mediaPlayer[i]->IsPaused()) {
					mediaPlayer[i]->Play();
				}
				else {
					//미디어 플레이어에서 실행할 비디오 소스 파일을 연다
					mediaPlayer[i]->OpenSource(videoSource[0]);
					//비디오 소스를 플레이한다
					mediaPlayer[i]->Play();
				}
			}
		}
	}
}

void AMyMovieActor::StopMovie() {
	if (mediaPlayer[0] != nullptr) {
		mediaPlayer[0]->Close();
	}
}

void AMyMovieActor::ReverseMovie(float second) {
	if (mediaPlayer[0]->IsPlaying()) {
		FTimespan modifiedTime = mediaPlayer[0]->GetTime() - FTimespan(0, 0, second);
		mediaPlayer[0]->Seek(modifiedTime);
	}
}

void AMyMovieActor::ForwardMovie(float second) {
	if (mediaPlayer[0]->IsPlaying()) {
		FTimespan modifiedTime = mediaPlayer[0]->GetTime() + FTimespan(0, 0, second);
		mediaPlayer[0]->Seek(modifiedTime);
	}
}