// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveComponent.h"
#include "VR_Player.h"
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include <Components/CapsuleComponent.h>

// Sets default values for this component's properties
UMoveComponent::UMoveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMoveComponent::BeginPlay()
{
	Super::BeginPlay();
	//주인인 플레이어 클래스를 캐싱한다
	player = Cast<AVR_Player>(GetOwner());
	//현재 월드 클랫스를 캐싱한다
	currentWorld = GetWorld();
}


// Called every frame
void UMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bIsShowLine) {
		DrawMoveLine();
	}
}

void UMoveComponent::SetupPlayerInputComponent(class UEnhancedInputComponent* PlayerInputComponent) {
	//AVR_Player* player = Cast<AVR_Player>(GetOwner());
	PlayerInputComponent->BindAction(leftThumbStick, ETriggerEvent::Triggered, this, &UMoveComponent::Move);
	PlayerInputComponent->BindAction(rightThumbStick, ETriggerEvent::Triggered, this, &UMoveComponent::RotateAxis);
	PlayerInputComponent->BindAction(leftTrigger, ETriggerEvent::Triggered, this, &UMoveComponent::ShowLine);
	PlayerInputComponent->BindAction(leftTrigger, ETriggerEvent::Completed, this, &UMoveComponent::HideLine);
}

void UMoveComponent::Move(const struct FInputActionValue& value) {
	FVector2D axis = value.Get<FVector2D>();
	FVector direction = FVector(axis.X, axis.Y, 0.0f);

	player->AddMovementInput(direction.GetSafeNormal(), 1, false);

	/*player->AddControllerPitchInput(-axis.Y);
	player->AddControllerYawInput(axis.X);*/
}

void UMoveComponent::RotateAxis(const struct FInputActionValue& value) {
	FVector2D axis = value.Get<FVector2D>();
	player->AddControllerPitchInput(-axis.Y);
	player->AddControllerYawInput(axis.X);
}

void UMoveComponent::DrawMoveLine() {
	//이동할 곡선을 화면에 그린다
	float timeInterval = 0.02f;
	int32 count = 50;

	//FVector dir = FVector::ForwardVector + FVector::UpVector;
	//dir = player->leftHand->GetComponentTransform().TransformVector(dir.GetSafeNormal());
	//왼손 모델링의 로컬 좌표에 맞춰서 forward 방향과 up 방향을 다시 계산한다
	FVector handForward = FRotationMatrix(player->leftHand->GetComponentRotation()).GetUnitAxis(EAxis::Y);
	FVector handUp = FRotationMatrix(player->leftHand->GetComponentRotation()).GetUnitAxis(EAxis::X) * -1;

	FVector dir = handForward + handUp;

	//1. 이동할 위치 구간을 구한다 (TArray<FVector>)
	//1-1. P = p0 + vt - 0.5 * g * t^2
	lineLoc.Empty();
	for (int32 i = 0; i < count; i++) {
		//소요시간
		float timeTaken = timeInterval * i;
		//p = p0 + vt
		FVector prediction = player->leftHand->GetComponentLocation() + dir * timeTaken * range;
		//p.Z -= 0.5 * g * t^2
		prediction.Z += 0.5f * currentWorld->GetDefaultGravityZ() * timeTaken * timeTaken;
		////1-2. 0.1초씩 0~50번째 구간을 계산해서 배열에 넣는다
		lineLoc.Add(prediction);
		//라인트레이스를 구간마다 체크한다
		FHitResult hitinfo;
		if (i > 0) {
			if (currentWorld->LineTraceSingleByChannel(hitinfo, lineLoc[i - 1], lineLoc[i], ECC_Visibility)) {
				//부딪힌 지점을 마지막 좌표로 넣고 반복문을 강제 종료한다
				lineLoc.Add(hitinfo.ImpactPoint);
				break;
			}
		}
	}

	//2. 앞에서 구한 위치 마다 선으로 연결한 그림을 그린다 (DrawLine을 이용한다)
	for (int32 i = 0; i < lineLoc.Num() - 1; i++) {
		DrawDebugLine(currentWorld, lineLoc[i], lineLoc[i + 1], FColor::Green, false, -1, 0, 2);
	}
}

void UMoveComponent::Teleport() {
	//가리킨 지점으로 순간이동 한다 (죽시 좌표 이동)
	FVector targetLoc = lineLoc[lineLoc.Num() - 1];
	targetLoc.Z += player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	if (lineLoc.Num() > 0) {
		player->SetActorLocation(targetLoc, false, nullptr, ETeleportType::TeleportPhysics);

	}
}

void UMoveComponent::ShowLine() {
	bIsShowLine = true;
}
void UMoveComponent::HideLine() {
	bIsShowLine = false;
	Teleport();
}