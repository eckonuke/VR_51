// Fill out your copyright notice in the Description page of Project Settings.


#include "VR_Character.h"
#include <Camera/CameraComponent.h>
#include <MotionControllerComponent.h>
#include <HeadMountedDisplayFunctionLibrary.h>
#include "IKAnimInstance.h"

// Sets default values
AVR_Character::AVR_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//ī�޶� ������Ʈ
	compCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	compCamera->SetupAttachment(RootComponent);

	//������ ��Ʈ�ѷ�
	rightController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("controllerRight"));
	rightController->SetupAttachment(RootComponent);
	//��� �ҽ� ������
	rightController->MotionSource = "Right";
	//�޼�
	leftController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("controllerLeft"));
	leftController->SetupAttachment(RootComponent);
	//��� �ҽ� ����
	leftController->MotionSource = "Left";
}

// Called when the game starts or when spawned
void AVR_Character::BeginPlay()
{
	Super::BeginPlay();
	//��� ����� ���� ��ġ�� �����Ѵ�.
	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called every frame
void AVR_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//��� ��Ʈ�ѷ��� ��ġ���� ȸ�� ���� �ִϸ��̼� �������Ʈ ������ �����Ѵ�
	UIKAnimInstance* myAnim = Cast<UIKAnimInstance>(GetMesh()->GetAnimInstance());
	if (myAnim != nullptr) {
		myAnim->controllerLocation_Left = leftController->GetComponentLocation();
		myAnim->controllerLocation_Right = rightController->GetComponentLocation();

		myAnim->controllerRotation_Left = leftController->GetComponentRotation();
		myAnim->controllerRotation_Right = rightController->GetComponentRotation();
	}
}

// Called to bind functionality to input
void AVR_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

