// Fill out your copyright notice in the Description page of Project Settings.


#include "XylophoneActor.h"
#include <Components/SceneComponent.h>
#include "KeyboardActor.h"
#include <Components/ChildActorComponent.h>

// Sets default values
AXylophoneActor::AXylophoneActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(rootComp);

	for (int32 i = 0; i < 8; i++) {
		FName componentName = FName(*FString::Printf(TEXT("Keyboard_%d"), i + 1));
		childComps.Add(CreateDefaultSubobject<UChildActorComponent>(componentName));
		childComps[i]->SetupAttachment(RootComponent);
	}
}

// Called when the game starts or when spawned
void AXylophoneActor::BeginPlay()
{
	Super::BeginPlay();
	//for (int32 i = 0; i < childComps.Num(); i++) {
	//	childComps[i]->SetChildActorClass(keyboard);
	//}
}

// Called every frame
void AXylophoneActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

