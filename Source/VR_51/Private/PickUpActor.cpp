// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpActor.h"
#include <Components/SphereComponent.h>
#include <Components/StaticMeshComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>

// Sets default values
APickUpActor::APickUpActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//Tick이 필요없는 클래스라면 Tick을 꺼주는게 좋다 최적화에 좋다
	PrimaryActorTick.bCanEverTick = false;
	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	SetRootComponent(sphereComp);
	sphereComp->SetCollisionProfileName(TEXT("PickUp"));
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Compoenent"));
	meshComp->SetupAttachment(sphereComp);
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void APickUpActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickUpActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

