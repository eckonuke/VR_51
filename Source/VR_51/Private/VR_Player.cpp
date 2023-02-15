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

	//카메라 컴포넌트
	compCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	compCamera->SetupAttachment(RootComponent);

	//게이즈 포인터 UI
	gazePointer = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gaze Pointer"));
	gazePointer->SetupAttachment(compCamera);
	gazePointer->SetRelativeLocation(FVector(300, 0, 0));

	//머리 모양
	headMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Head"));
	headMesh->SetupAttachment(compCamera);
	headMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//오른손 컨트롤러
	rightController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("controllerRight"));
	rightController->SetupAttachment(RootComponent);
	//모션 소스 오른쪽
	rightController->MotionSource = "Right";
	//오른손 모양
	rightHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("rightHand"));
	rightHand->SetupAttachment(rightController);
	rightHand->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	rightHand->SetRelativeRotation(FRotator(25.0f, 180.0f, 90.0f));

	widgetPointer_right = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("Widget Pointer"));
	widgetPointer_right->SetupAttachment(rightController);

	//오른손 로그
	rightLog = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Right Log Text"));
	rightLog->SetupAttachment(rightController);
	rightLog->SetRelativeRotation(FRotator(25.0f, 0.0f, 90.0f));
	rightLog->SetTextRenderColor(FColor::Yellow);
	rightLog->SetHorizontalAlignment(EHTA_Center);
	rightLog->SetVerticalAlignment(EVRTA_TextCenter);

	//왼손
	leftController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("controllerLeft"));
	leftController->SetupAttachment(RootComponent);
	//모션 소스 왼쪽
	leftController->MotionSource = "Left";
	//왼손 모양
	leftHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("leftHand"));
	leftHand->SetupAttachment(leftController);
	leftHand->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	leftHand->SetRelativeRotation(FRotator(-25.0f, 180.0f, 90.0f));

	//왼손 로그
	leftLog = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Left Log Text"));
	leftLog->SetupAttachment(leftController);
	leftLog->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	leftLog->SetTextRenderColor(FColor::Yellow);
	leftLog->SetHorizontalAlignment(EHTA_Center);
	leftLog->SetVerticalAlignment(EVRTA_TextCenter);

	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = true;

	//내가 플레이하는 플레이어 선택
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	//액터 MoveComponent 추가
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
	//헤드 장비의 기준 위치를 설정한다.
	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(trackOrigin.GetValue());

	//1. 플레이어 컨트롤러를 가져온다
	APlayerController* playerCon = GetWorld()->GetFirstPlayerController();

	//2. 플레이어 컨트롤러에서 EnhancedInputLocalPlayerSubsystem를 가져온다
	UEnhancedInputLocalPlayerSubsystem* subsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerCon->GetLocalPlayer());

	//3. 가져온 Subsystem에 IMC를 등록한다 (우선순위 0번)
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
		//왼손 입력
		/*enhancedInputComponent->BindAction(leftInputs[0], ETriggerEvent::Triggered, this, &AVR_Player::OnTriggerLeft);
		enhancedInputComponent->BindAction(leftInputs[0], ETriggerEvent::Completed, this, &AVR_Player::OnTriggerLeft);*/
		//enhancedInputComponent->BindAction(leftInputs[1], ETriggerEvent::Triggered, this, &AVR_Player::RotateAxis);
		enhancedInputComponent->BindAction(leftInputs[2], ETriggerEvent::Triggered, this, &AVR_Player::OnGripLeft);
		enhancedInputComponent->BindAction(leftInputs[3], ETriggerEvent::Triggered, this, &AVR_Player::OnButtonXLeft);
		enhancedInputComponent->BindAction(leftInputs[4], ETriggerEvent::Triggered, this, &AVR_Player::OnButtonYLeft);
		enhancedInputComponent->BindAction(leftInputs[5], ETriggerEvent::Triggered, this, &AVR_Player::OnButtonMenuLeft);

		//오른손 입력
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
	//다시 플레이어의 원점으로 방향 설정
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AVR_Player::OnTriggerLeft(const FInputActionValue& value) {
	float val = value.Get<float>();

	//왼손 로그에 값을 출력한다
	FString msg = FString::Printf(TEXT("%f"), val);
	leftLog->SetText(FText::FromString(msg));
}
void AVR_Player::OnGripLeft(const struct FInputActionValue& value) {
	//왼손 로그에 값을 출력한다
	FString msg = FString(__FUNCTION__);
	leftLog->SetText(FText::FromString(msg));
}
void AVR_Player::OnButtonXLeft(const struct FInputActionValue& value) {
	//왼손 로그에 값을 출력한다
	FString msg = FString(__FUNCTION__);
	leftLog->SetText(FText::FromString(msg));
}
void AVR_Player::OnButtonYLeft(const struct FInputActionValue& value) {
	//왼손 로그에 값을 출력한다
	FString msg = FString(__FUNCTION__);
	leftLog->SetText(FText::FromString(msg));
}
void AVR_Player::OnButtonMenuLeft(const struct FInputActionValue& value) {
	//왼손 로그에 값을 출력한다
	FString msg = FString(__FUNCTION__);
	leftLog->SetText(FText::FromString(msg));
}

void AVR_Player::OnTriggerRight(const struct FInputActionValue& value) {
	//오른손 로그에 값을 출력한다
	FString msg = FString(__FUNCTION__);
	rightLog->SetText(FText::FromString(msg));
}
void AVR_Player::OnGripRight(const struct FInputActionValue& value) {
	//오른손 로그에 값을 출력한다
	FString msg = FString(__FUNCTION__);
	rightLog->SetText(FText::FromString(msg));
}
void AVR_Player::OnButtonARight(const struct FInputActionValue& value) {
	//오른손 로그에 값을 출력한다
	FString msg = FString(__FUNCTION__);
	rightLog->SetText(FText::FromString(msg));
}
void AVR_Player::OnButtonBRight(const struct FInputActionValue& value) {
	//오른손 로그에 값을 출력한다
	FString msg = FString(__FUNCTION__);
	rightLog->SetText(FText::FromString(msg));
}
void AVR_Player::OnButtonMenuRight(const struct FInputActionValue& value) {
	//오른손 로그에 값을 출력한다
	FString msg = FString(__FUNCTION__);
	rightLog->SetText(FText::FromString(msg));
}