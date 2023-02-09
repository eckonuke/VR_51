// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetPointerComponent.h"
#include "EnhancedInputComponent.h"
#include "VR_Player.h"
#include <UMG/Public/Components/WidgetInteractionComponent.h>


// Sets default values for this component's properties
UWidgetPointerComponent::UWidgetPointerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWidgetPointerComponent::BeginPlay()
{
	Super::BeginPlay();
	player = Cast<AVR_Player>(GetOwner());
	
}


// Called every frame
void UWidgetPointerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
}

void UWidgetPointerComponent::SetupPlayerInputComponent(class UEnhancedInputComponent* PlayerInputComponent) {
	PlayerInputComponent->BindAction(right_B, ETriggerEvent::Started, this, &UWidgetPointerComponent::ClickWidget);
	PlayerInputComponent->BindAction(right_B, ETriggerEvent::Completed, this, &UWidgetPointerComponent::ReleaseWidget);
}

// UI 요소에 대한 클릭 처리 함수
void UWidgetPointerComponent::ClickWidget() {
	player->widgetPointer_right->PressPointerKey(EKeys::LeftMouseButton);
}

void UWidgetPointerComponent::ReleaseWidget(){
	player->widgetPointer_right->ReleasePointerKey(EKeys::LeftMouseButton);
}
