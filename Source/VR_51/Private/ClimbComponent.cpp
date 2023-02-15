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
		//���� ��ġ���� ���� ��ġ�� �� ���͸� ���Ѵ�
		FVector delta = climb_start_right - player->rightController->GetComponentLocation();
		//�տ��� ���� ���͸� �÷��̾� ������ ��ġ�� ���Ѵ�
		player->AddActorWorldOffset(delta);
	}
}

void UClimbComponent::SetupPlayerInputComponent(class UEnhancedInputComponent* enhancedInputComponent) {
	enhancedInputComponent->BindAction(grab_r, ETriggerEvent::Triggered, this, &UClimbComponent::startClimb);
	enhancedInputComponent->BindAction(grab_r, ETriggerEvent::Completed, this, &UClimbComponent::endClimb);
}

void UClimbComponent::startClimb() {
	canClimb = true;
	//�÷��̾��� �߷��� off�Ѵ�
	player->GetCharacterMovement()->GravityScale = 0;
	//���ʷ� ���� ��ġ ���͸� �����Ѵ�
	climb_start_right = player->rightController->GetComponentLocation();
	//�ö󰡴� ���� ���·� ��ȯ�Ѵ�.
	bClimbing_right = true;
}

void UClimbComponent::endClimb() {
	canClimb = false;
	player->GetCharacterMovement()->GravityScale = 1.0f;
	bClimbing_right = false;
}

