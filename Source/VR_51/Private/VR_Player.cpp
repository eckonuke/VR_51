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

// Sets default values
AVR_Player::AVR_Player()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//ī�޶� ������Ʈ
	compCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	compCamera->SetupAttachment(RootComponent);

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

	//������ �α�
	rightLog = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Right Log Text"));
	rightLog->SetupAttachment(rightController);
	rightLog->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
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
		enhancedInputComponent->BindAction(leftInputs[0], ETriggerEvent::Triggered, this, &AVR_Player::OnTriggerLeft);
		enhancedInputComponent->BindAction(leftInputs[0], ETriggerEvent::Completed, this, &AVR_Player::OnTriggerLeft);
		enhancedInputComponent->BindAction(leftInputs[1], ETriggerEvent::Triggered, this, &AVR_Player::RotateAxis);
	}
}

void AVR_Player::OnTriggerLeft(const FInputActionValue& value) {
	float val = value.Get<float>();

	//�޼� �α׿� ���� ����Ѵ�
	FString msg = FString::Printf(TEXT("%f"), val);
	leftLog->SetText(FText::FromString(msg));
}

void AVR_Player::RotateAxis(const struct FInputActionValue& value) {
	FVector2D axis = value.Get<FVector2D>();

	AddControllerPitchInput(-axis.Y);
	AddControllerYawInput(axis.X);
}