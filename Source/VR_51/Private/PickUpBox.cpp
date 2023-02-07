// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpBox.h"
#include <Components/BoxComponent.h>
#include <Components/StaticMeshComponent.h>

APickUpBox::APickUpBox() {
	rootComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	SetRootComponent(rootComp);
	rootComp->SetCollisionProfileName(collisionProfileName);

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	meshComp->SetupAttachment(rootComp);
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
