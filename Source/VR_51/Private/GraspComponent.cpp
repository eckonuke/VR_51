// Fill out your copyright notice in the Description page of Project Settings.


#include "GraspComponent.h"
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include "VR_Player.h"
#include "VRHandAnimInstance.h"
#include "PickUpActor.h"
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include <MotionControllerComponent.h>
#include "PickUpBox.h"
#include <Components/BoxComponent.h>
#include <Haptics/HapticFeedbackEffect_Base.h>

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

	//플레이어 캐릭터와 오른손 애니메이션 인스턴스를 캐싱한다
	player = Cast<AVR_Player>(GetOwner());
	rightHandAnim = Cast<UVRHandAnimInstance>(player->rightHand->GetAnimInstance());

	if (rightHandAnim != nullptr)
		ResetRightFingers();
}


// Called every frame
void UGraspComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

void UGraspComponent::SetupPlayerInputComponent(class UEnhancedInputComponent* PlayerInputComponent) {
	PlayerInputComponent->BindAction(grip_right, ETriggerEvent::Started, this, &UGraspComponent::GripRightAction);
	PlayerInputComponent->BindAction(grip_right, ETriggerEvent::Completed, this, &UGraspComponent::GripRightRelease);
	PlayerInputComponent->BindAction(trigger_right, ETriggerEvent::Started, this, &UGraspComponent::TriggerRightAction);
	PlayerInputComponent->BindAction(trigger_right, ETriggerEvent::Completed, this, &UGraspComponent::TriggerRightAction);
	PlayerInputComponent->BindAction(trigger_right_touch, ETriggerEvent::Started, this, &UGraspComponent::TrggerRightTouch);
	PlayerInputComponent->BindAction(trigger_right_touch, ETriggerEvent::Completed, this, &UGraspComponent::TrggerRightTouchEnd);
	PlayerInputComponent->BindAction(thumb_right_touch, ETriggerEvent::Started, this, &UGraspComponent::ThumbRightTouch);
	PlayerInputComponent->BindAction(thumb_right_touch, ETriggerEvent::Completed, this, &UGraspComponent::ThumbRightTouchEnd);
	PlayerInputComponent->BindAction(btnA, ETriggerEvent::Started, this, &UGraspComponent::ReadyToShoot);
	PlayerInputComponent->BindAction(btnA, ETriggerEvent::Completed, this, &UGraspComponent::ReadyToShoot);
}

void UGraspComponent::GripRightAction(const struct FInputActionValue& value) {
	rightHandAnim->PoseAlphaGrasp = value.Get<float>();
	GrabObject(player->rightHand);
	
	//오른손에 진동 효과 주기
	GetWorld()->GetFirstPlayerController()->PlayHapticEffect(grabHaptic, EControllerHand::Right, 1, false);
}

void UGraspComponent::GripRightRelease(const struct FInputActionValue& value) {
	bIsGrab = false;
	//ReleaseObject(player->rightHand);
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

//손가락 애니메이션 변수의 기본 값으로 설정
void UGraspComponent::ResetRightFingers() {
	rightHandAnim->PoseAlphaGrasp = 0;
	rightHandAnim->PoseAlphaIndexCurl = 0;
	rightHandAnim->PoseAlphaPoint = 1;
	rightHandAnim->PoseAlphaThumbUp = 1;
}
//물체를 잡는 함수
void UGraspComponent::GrabObject(USkeletalMeshComponent* selectHand) {
	FCollisionQueryParams params;
	params.AddIgnoredActor(player);
	if (myGrabType == EGrabType::ELine) {
		//방법1 - LineTrace 방식
		FVector startLoc = selectHand->GetComponentLocation();
		FVector endLoc = startLoc + selectHand->GetRightVector() * grabDistance;
		FHitResult hitInfo;

		if (GetWorld()->LineTraceSingleByProfile(hitInfo, startLoc, endLoc, TEXT("PickUp")) && grabedObject == nullptr) {
			//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, hitInfo.GetActor()->GetName());
			hitInfo.GetActor()->AttachToComponent(selectHand, FAttachmentTransformRules::KeepWorldTransform);
			grabedObject = Cast<APickUpActor>(hitInfo.GetActor());
		}
	}
	else if (myGrabType == EGrabType::ESweep) {
		//방법2 - SphereTrace 방식
		FVector center = selectHand->GetComponentLocation();
		FHitResult hitInfo;
		if (GetWorld()->SweepSingleByProfile(hitInfo, center, center, FQuat::Identity, TEXT("PickUp"), FCollisionShape::MakeSphere(grabDistance), params) && grabedObject == nullptr) {
			grabedObject = Cast<APickUpActor>(hitInfo.GetActor());
			if (IsValid(grabedObject)) {
				//만일, 검색된 물체의 physics가 simulate 중이라면...
				UBoxComponent* boxComp = Cast<UBoxComponent>(grabedObject->GetRootComponent());
				//대상의 physics를 꺼준다
				if (boxComp != nullptr) {
					physicsState = boxComp->IsSimulatingPhysics();
					boxComp->SetSimulatePhysics(false);
				}
				hitInfo.GetActor()->AttachToComponent(selectHand, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("GrabPoint"));
			}
		}
	}
	else if (myGrabType == EGrabType::EOverlap)
	{
		//방법3 - 오버랩 방식으로 검사하기
		FVector center = selectHand->GetComponentLocation();
		TArray<FOverlapResult> hitInfos;
		FCollisionObjectQueryParams objectQuery;
		objectQuery.AddObjectTypesToQuery(ECC_WorldDynamic);

		if (GetWorld()->OverlapMultiByObjectType(hitInfos, center, FQuat::Identity, objectQuery, FCollisionShape::MakeSphere(grabDistance), params))
		{
			// 부딪힌 모든 대상을 순회한다.
			for (FOverlapResult& hitInfo : hitInfos)
			{
				// 검사 대상 중에 PickUpActor 클래스인 경우에만 손에 붙인다.
				APickUpActor* picks = Cast<APickUpActor>(hitInfo.GetActor());
				if (picks != nullptr)
				{
					hitInfo.GetActor()->AttachToComponent(selectHand, FAttachmentTransformRules::KeepWorldTransform);
				}
			}
		}
	}
	bIsGrab = true;
}

void UGraspComponent::ReleaseObject(USkeletalMeshComponent* selectHand, FVector torque) {
	//만일 잡고 있던 물체가 있다면...
	if (grabedObject != nullptr) {
		//잡고 있던 물체를 떼어낸다.
		grabedObject->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		//물체의 본래 피직스 on/off 여부들 되돌려준다
		UBoxComponent* boxComp = Cast<UBoxComponent>(grabedObject->GetRootComponent());
		if (boxComp != nullptr) {
			boxComp->SetSimulatePhysics(physicsState);
		}
		//날라가야할 방향을 구한다
		throwDirection.Normalize();
		//구한 방향대로 충격을 가한다.
		boxComp->AddImpulse(throwDirection * throwPower);
		//FVector dir = player->rightController->GetPhysicsLinearVelocity();
		//dir.Normalize();
		//FVector dir = selectHand->GetPhysicsLinearVelocity();
		//grabedObject->sphereComp->AddImpulse(dir * throwPower);

		//회전속도
		//FVector dtheta = player->rightController->GetPhysicsAngularVelocityInRadians();
		boxComp->AddTorqueInDegrees(torque * torquePower, NAME_None, true);

		//grabedObject 포인터 변수를 nullptr로 변경한
		grabedObject = nullptr;


	}
}

void UGraspComponent::DrawGrabRange()
{
	if (myGrabType == EGrabType::ELine) {
		//선 그리기 (Line 방식)
		FVector startLoc = player->rightHand->GetComponentLocation();
		FVector endLoc = startLoc + player->rightHand->GetRightVector() * grabDistance;
		DrawDebugLine(GetWorld(), startLoc, endLoc, FColor::Cyan, false, -1, 0, 2);

	}
	else if (myGrabType == EGrabType::EOverlap || myGrabType == EGrabType::ESweep) {
		//선그리기 (Sphere 방식)
		DrawDebugSphere(GetWorld(), player->rightHand->GetComponentLocation(), grabDistance, 30, FColor::Magenta, false, -1, 0, 1);
	}

}

void UGraspComponent::ReadyToShoot() {
	if (grabedObject != nullptr) {
		if (!bIsReady) {
			//DrawGrabRange();
			prevLocation = player->rightController->GetComponentLocation();
			prevForward = player->rightController->GetForwardVector();
		}
		else {
			//던질 방향
			throwDirection = player->rightController->GetComponentLocation() - prevLocation;

			//회전값
			FVector rotAxis = FVector::CrossProduct(prevForward, player->rightController->GetForwardVector());
			float angle = FMath::Acos(FVector::DotProduct(prevForward, player->rightController->GetForwardVector()));
			angle = FMath::RadiansToDegrees(angle);


			ReleaseObject(player->rightHand, rotAxis * angle);
			DrawDebugLine(GetWorld(), player->rightController->GetComponentLocation(), player->rightController->GetComponentLocation() + throwDirection * 50, FColor::Red, false, 5, 0, 3);
		}
		bIsReady = !bIsReady;
	}
}
