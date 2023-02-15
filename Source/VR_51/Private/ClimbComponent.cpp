// Fill out your copyright notice in the Description page of Project Settings.


#include "ClimbComponent.h"
#include "VR_Player.h"
#include <Components/CapsuleComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <MotionControllerComponent.h>
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>

// Sets default values for this component's properties
UClimbComponent::UClimbComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UClimbComponent::BeginPlay()
{
	Super::BeginPlay();
	player = Cast<AVR_Player>(GetOwner());
}


// Called every frame
void UClimbComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!canClimb) {
		return;
	}

	if (bClimbing_right) {
		//최초 위치에서 손의 위치를 뺀 벡터를 구한다
		FVector delta = climb_start_right - player->rightController->GetComponentLocation();
		//앞에서 구한 벡터를 플레이어 액터의 위치에 더한다
		player->AddActorWorldOffset(delta);
	}
}

void UClimbComponent::SetupPlayerInputComponent(class UEnhancedInputComponent* enhancedInputComponent) {
	enhancedInputComponent->BindAction(grab_r, ETriggerEvent::Triggered, this, &UClimbComponent::startClimb);
	enhancedInputComponent->BindAction(grab_r, ETriggerEvent::Completed, this, &UClimbComponent::endClimb);
}

void UClimbComponent::startClimb() {
	canClimb = true;
	//플레이어의 중력을 off한다
	player->GetCharacterMovement()->GravityScale = 0;
	//최초로 잡은 위치 벡터를 저장한다
	climb_start_right = player->rightController->GetComponentLocation();
	//올라가는 중인 상태로 전환한다.
	bClimbing_right = true;
}

void UClimbComponent::endClimb() {
	canClimb = false;
	player->GetCharacterMovement()->GravityScale = 1.0f;
	bClimbing_right = false;
}

