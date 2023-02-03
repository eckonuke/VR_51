// Fill out your copyright notice in the Description page of Project Settings.


#include "GraspComponent.h"
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include "VR_Player.h"
#include "VRHandAnimInstance.h"

// Sets default values for this component's properties
UGraspComponent::UGraspComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGraspComponent::BeginPlay() {
	Super::BeginPlay();

	//�÷��̾� ĳ���Ϳ� ������ �ִϸ��̼� �ν��Ͻ��� ĳ���Ѵ�
	player = Cast<AVR_Player>(GetOwner());
	rightHandAnim = Cast<UVRHandAnimInstance>(player->rightHand->GetAnimInstance());

	if(rightHandAnim !=nullptr)
		ResetRightFingers();
}


// Called every frame
void UGraspComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}

void UGraspComponent::SetupPlayerInputComponent(class UEnhancedInputComponent* PlayerInputComponent) {
	PlayerInputComponent->BindAction(grip_right, ETriggerEvent::Triggered, this, &UGraspComponent::GripRightAction);
	PlayerInputComponent->BindAction(grip_right, ETriggerEvent::Completed, this, &UGraspComponent::GripRightAction);
	PlayerInputComponent->BindAction(trigger_right, ETriggerEvent::Triggered, this, &UGraspComponent::TriggerRightAction);
	PlayerInputComponent->BindAction(trigger_right, ETriggerEvent::Completed, this, &UGraspComponent::TriggerRightAction);
	PlayerInputComponent->BindAction(trigger_right_touch, ETriggerEvent::Triggered, this, &UGraspComponent::TrggerRightTouch);
	PlayerInputComponent->BindAction(trigger_right_touch, ETriggerEvent::Completed, this, &UGraspComponent::TrggerRightTouchEnd);
	PlayerInputComponent->BindAction(thumb_right_touch, ETriggerEvent::Triggered, this, &UGraspComponent::ThumbRightTouch);
	PlayerInputComponent->BindAction(thumb_right_touch, ETriggerEvent::Completed, this, &UGraspComponent::ThumbRightTouchEnd);

}

void UGraspComponent::GripRightAction(const struct FInputActionValue& value) {
	rightHandAnim->PoseAlphaGrasp = value.Get<float>();
	GrabObject(player->rightHand);
}
void UGraspComponent::TriggerRightAction(const struct FInputActionValue& value) {
	rightHandAnim->PoseAlphaIndexCurl = value.Get<float>();
}
void UGraspComponent::TrggerRightTouch() {
	rightHandAnim->PoseAlphaPoint = 0;
}
void UGraspComponent::TrggerRightTouchEnd() {
	rightHandAnim->PoseAlphaPoint = 1;
}
void UGraspComponent::ThumbRightTouch() {
	rightHandAnim->PoseAlphaThumbUp = 0;
}
void UGraspComponent::ThumbRightTouchEnd() {
	rightHandAnim->PoseAlphaThumbUp = 1;
}

//�հ��� �ִϸ��̼� ������ �⺻ ������ ����
void UGraspComponent::ResetRightFingers() {
	rightHandAnim->PoseAlphaGrasp = 0;
	rightHandAnim->PoseAlphaIndexCurl = 0;
	rightHandAnim->PoseAlphaPoint = 1;
	rightHandAnim->PoseAlphaThumbUp = 1;
}
//��ü�� ��� �Լ�
void UGraspComponent::GrabObject(USkeletalMeshComponent* selectHand) {
	//���1 - LineTrace���
	FVector startLoc = selectHand->GetComponentLocation();
	FVector endLoc = startLoc + selectHand->GetRightVector() * grabDistance;
	FHitResult hitInfo;

	if (GetWorld()->LineTraceSingleByProfile(hitInfo, startLoc, endLoc, TEXT("PickUp"))) {
		hitInfo.GetActor()->AttachToComponent(selectHand, FAttachmentTransformRules::KeepWorldTransform);
	}

}
