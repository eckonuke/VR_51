// Fill out your copyright notice in the Description page of Project Settings.


#include "GazeComponent.h"
#include "VR_Player.h"
#include <Camera/CameraComponent.h>
#include "MyMovieActor.h"
#include "EngineUtils.h"

// Sets default values for this component's properties
UGazeComponent::UGazeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UGazeComponent::BeginPlay() {
	Super::BeginPlay();
	player = Cast<AVR_Player>(GetOwner());
	
	for (TActorIterator<AMyMovieActor> mp(GetWorld()); mp; ++mp) {
		 moviePlayer = *mp;
	}
}


// Called every frame
void UGazeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	ChangeSize(min, max);
	FVector startLoc = player->compCamera->GetComponentLocation();
	FVector endLoc = startLoc + player->compCamera->GetForwardVector() * 1000.0f;
	FHitResult hitInfo;
	FCollisionQueryParams params;
	params.AddIgnoredActor(player);
	bool isHit= GetWorld()->LineTraceSingleByChannel(hitInfo, startLoc, endLoc, ECC_GameTraceChannel2, params);
	increase = isHit;
	if (isHit && moviePlayer != nullptr && canChange) {
		canChange = !canChange;
		moviePlayer->ChangeMovie();
	}
}

void UGazeComponent::ChangeSize(FVector minSize, FVector maxSize) {
	//점진적으로 크기가 줄어들고 늘어난다
	//만일, increase라면 
	//현재 사이즈에서 maxSize로 증가시킨다
	//그렇지 않다면
	//현재 사이즈에서 minSize로 감소시킨다
	currentSize = increase ? currentSize + GetWorld()->GetDeltaSeconds() : currentSize - GetWorld()->GetDeltaSeconds();
	currentSize = FMath::Clamp(currentSize, 0.0f, 1.0f);
	FVector targetSize = FMath::Lerp(minSize, maxSize, currentSize);
	player->gazePointer->SetRelativeScale3D(targetSize);
}
