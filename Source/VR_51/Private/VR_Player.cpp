// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "VR_Player.h"
#include <MotionControllerComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Camera/CameraComponent.h>
#include <Components/SphereComponent.h>
#include <HeadMountedDisplayFunctionLibrary.h>
#include <Components/TextRenderComponent.h>
#include "Components/SkeletalMeshComponent.h"
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h>
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h>
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputMappingContext.h>
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputAction.h>
#include "MoveComponent.h"
#include "GraspComponent.h"
#include <UMG/Public/Components/WidgetInteractionComponent.h>
#include "WidgetPointerComponent.h"
#include "GazeComponent.h"
#include "ClimbComponent.h"

// Sets default values
AVR_Player::AVR_Player()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//ī�޶� ������Ʈ
	compCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	compCamera->SetupAttachment(RootComponent);

	//������ ������ UI
	gazePointer = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gaze Pointer"));
	gazePointer->SetupAttachment(compCamera);
	gazePointer->SetRelativeLocation(FVector(300, 0, 0));

	//�Ӹ� ���
	headMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Head"));
	headMesh->SetupAttachment(compCamera);
	headMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//������ ��Ʈ�ѷ�
	rightController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("controllerRight"));
	rightController->SetupAttachment(RootComponent);
	//��� �ҽ� ������
	rightController->MotionSource = "Right";
	//������ ���
	rightHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("rightHand"));
	rightHand->SetupAttachment(rightController);
	rightHand->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	rightHand->SetRelativeRotation(FRotator(25.0f, 180.0f, 90.0f));

	widgetPointer_right = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("Widget Pointer"));
	widgetPointer_right->SetupAttachment(rightController);

	//������ �α�
	rightLog = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Right Log Text"));
	rightLog->SetupAttachment(rightController);
	rightLog->SetRelativeRotation(FRotator(25.0f, 0.0f, 90.0f));
	rightLog->SetTextRenderColor(FColor::Yellow);
	rightLog->SetHorizontalAlignment(EHTA_Center);
	rightLog->SetVerticalAlignment(EVRTA_TextCenter);

	//�޼�
	leftController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("controllerLeft"));
	leftController->SetupAttachment(RootComponent);
	//��� �ҽ� ����
	leftController->MotionSource = "Left";
	//�޼� ���
	leftHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("leftHand"));
	leftHand->SetupAttachment(leftController);
	leftHand->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	leftHand->SetRelativeRotation(FRotator(-25.0f, 180.0f, 90.0f));

	//�޼� �α�
	leftLog = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Left Log Text"));
	leftLog->SetupAttachment(leftController);
	leftLog->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	leftLog->SetTextRenderColor(FColor::Yellow);
	leftLog->SetHorizontalAlignment(EHTA_Center);
	leftLog->SetVerticalAlignment(EVRTA_TextCenter);

	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = true;

	//���� �÷����ϴ� �÷��̾� ����
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	//���� MoveComponent �߰�
	moveComp = CreateDefaultSubobject<UMoveComponent>(TEXT("MoveComponent"));
	graspComp = CreateDefaultSubobject<UGraspComponent>(TEXT("GraspComponent"));
	widgetPointerComp = CreateDefaultSubobject<UWidgetPointerComponent>(TEXT("Widget Pointer Component"));
	gazeComp = CreateDefaultSubobject<UGazeComponent>(TEXT("Gaze Component"));
	climbComp = CreateDefaultSubobject<UClimbComponent>(TEXT("Climb Component"));
}

// Called when the game starts or when spawned
void AVR_Player::BeginPlay()
{
	Super::BeginPlay();
	//��� ����� ���� ��ġ�� �����Ѵ�.
	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(trackOrigin.GetValue());

	//1. �÷��̾� ��Ʈ�ѷ��� �����´�
	APlayerController* playerCon = GetWorld()->GetFirstPlayerController();

	//2. �÷��̾� ��Ʈ�ѷ����� EnhancedInputLocalPlayerSubsystem�� �����´�
	UEnhancedInputLocalPlayerSubsystem* subsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerCon->GetLocalPlayer());

	//3. ������ Subsystem�� IMC�� ����Ѵ� (�켱���� 0��)
	subsys->AddMappingContext(myMapping, 0);
}

// Called every frame
void AVR_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVR_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (enhancedInputComponent != nullptr) {
		//�޼� �Է�
		/*enhancedInputComponent->BindAction(leftInputs[0], ETriggerEvent::Triggered, this, &AVR_Player::OnTriggerLeft);
		enhancedInputComponent->BindAction(leftInputs[0], ETriggerEvent::Completed, this, &AVR_Player::OnTriggerLeft);*/
		//enhancedInputComponent->BindAction(leftInputs[1], ETriggerEvent::Triggered, this, &AVR_Player::RotateAxis);
		enhancedInputComponent->BindAction(leftInputs[2], ETriggerEvent::Triggered, this, &AVR_Player::OnGripLeft);
		enhancedInputComponent->BindAction(leftInputs[3], ETriggerEvent::Triggered, this, &AVR_Player::OnButtonXLeft);
		enhancedInputComponent->BindAction(leftInputs[4], ETriggerEvent::Triggered, this, &AVR_Player::OnButtonYLeft);
		enhancedInputComponent->BindAction(leftInputs[5], ETriggerEvent::Triggered, this, &AVR_Player::OnButtonMenuLeft);

		//������ �Է�
		/*enhancedInputComponent->BindAction(rightInputs[0], ETriggerEvent::Triggered, this, &AVR_Player::OnTriggerLeft);
		enhancedInputComponent->BindAction(rightInputs[0], ETriggerEvent::Completed, this, &AVR_Player::OnTriggerLeft);
		enhancedInputComponent->BindAction(rightInputs[1], ETriggerEvent::Triggered, this, &AVR_Player::RotateAxis);
		enhancedInputComponent->BindAction(rightInputs[2], ETriggerEvent::Triggered, this, &AVR_Player::OnGripRight);*/
		enhancedInputComponent->BindAction(rightInputs[0], ETriggerEvent::Started, this, &AVR_Player::Recenter);
		enhancedInputComponent->BindAction(rightInputs[3], ETriggerEvent::Triggered, this, &AVR_Player::OnButtonARight);
		//enhancedInputComponent->BindAction(rightInputs[4], ETriggerEvent::Triggered, this, &AVR_Player::OnButtonBRight);
		moveComp->SetupPlayerInputComponent(enhancedInputComponent);
		graspComp->SetupPlayerInputComponent(enhancedInputComponent);
		widgetPointerComp->SetupPlayerInputComponent(enhancedInputComponent);
		climbComp->SetupPlayerInputComponent(enhancedInputComponent);
	}
}

void AVR_Player::Recenter() {
	//�ٽ� �÷��̾��� �������� ���� ����
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AVR_Player::OnTriggerLeft(const FInputActionValue& value) {
	float val = value.Get<float>();

	//�޼� �α׿� ���� ����Ѵ�
	FString msg = FString::Printf(TEXT("%f"), val);
	leftLog->SetText(FText::FromString(msg));
}
void AVR_Player::OnGripLeft(const struct FInputActionValue& value) {
	//�޼� �α׿� ���� ����Ѵ�
	FString msg = FString(__FUNCTION__);
	leftLog->SetText(FText::FromString(msg));
}
void AVR_Player::OnButtonXLeft(const struct FInputActionValue& value) {
	//�޼� �α׿� ���� ����Ѵ�
	FString msg = FString(__FUNCTION__);
	leftLog->SetText(FText::FromString(msg));
}
void AVR_Player::OnButtonYLeft(const struct FInputActionValue& value) {
	//�޼� �α׿� ���� ����Ѵ�
	FString msg = FString(__FUNCTION__);
	leftLog->SetText(FText::FromString(msg));
}
void AVR_Player::OnButtonMenuLeft(const struct FInputActionValue& value) {
	//�޼� �α׿� ���� ����Ѵ�
	FString msg = FString(__FUNCTION__);
	leftLog->SetText(FText::FromString(msg));
}

void AVR_Player::OnTriggerRight(const struct FInputActionValue& value) {
	//������ �α׿� ���� ����Ѵ�
	FString msg = FString(__FUNCTION__);
	rightLog->SetText(FText::FromString(msg));
}
void AVR_Player::OnGripRight(const struct FInputActionValue& value) {
	//������ �α׿� ���� ����Ѵ�
	FString msg = FString(__FUNCTION__);
	rightLog->SetText(FText::FromString(msg));
}
void AVR_Player::OnButtonARight(const struct FInputActionValue& value) {
	//������ �α׿� ���� ����Ѵ�
	FString msg = FString(__FUNCTION__);
	rightLog->SetText(FText::FromString(msg));
}
void AVR_Player::OnButtonBRight(const struct FInputActionValue& value) {
	//������ �α׿� ���� ����Ѵ�
	FString msg = FString(__FUNCTION__);
	rightLog->SetText(FText::FromString(msg));
}
void AVR_Player::OnButtonMenuRight(const struct FInputActionValue& value) {
	//������ �α׿� ���� ����Ѵ�
	FString msg = FString(__FUNCTION__);
	rightLog->SetText(FText::FromString(msg));
}