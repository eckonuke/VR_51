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
	//카메라 컴포넌트
	compCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	compCamera->SetupAttachment(RootComponent);

	//오른손 컨트롤러
	rightController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("controllerRight"));
	rightController->SetupAttachment(RootComponent);
	//모션 소스 오른쪽
	rightController->MotionSource = "Right";
	//왼손
	leftController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("controllerLeft"));
	leftController->SetupAttachment(RootComponent);
	//모션 소스 왼쪽
	leftController->MotionSource = "Left";
}

// Called when the game starts or when spawned
void AVR_Character::BeginPlay()
{
	Super::BeginPlay();
	//헤드 장비의 기준 위치를 설정한다.
	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called every frame
void AVR_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//모션 컨트롤러의 위치값과 회전 값을 애니메이션 블루프린트 변수에 전달한다
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

