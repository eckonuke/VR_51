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
	//������ �÷��̾� Ŭ������ ĳ���Ѵ�
	player = Cast<AVR_Player>(GetOwner());
	//���� ���� Ŭ������ ĳ���Ѵ�
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
	//�̵��� ��� ȭ�鿡 �׸���
	float timeInterval = 0.02f;
	int32 count = 50;

	//FVector dir = FVector::ForwardVector + FVector::UpVector;
	//dir = player->leftHand->GetComponentTransform().TransformVector(dir.GetSafeNormal());
	//�޼� �𵨸��� ���� ��ǥ�� ���缭 forward ����� up ������ �ٽ� ����Ѵ�
	FVector handForward = FRotationMatrix(player->leftHand->GetComponentRotation()).GetUnitAxis(EAxis::Y);
	FVector handUp = FRotationMatrix(player->leftHand->GetComponentRotation()).GetUnitAxis(EAxis::X) * -1;

	FVector dir = handForward + handUp;

	//1. �̵��� ��ġ ������ ���Ѵ� (TArray<FVector>)
	//1-1. P = p0 + vt - 0.5 * g * t^2
	lineLoc.Empty();
	for (int32 i = 0; i < count; i++) {
		//�ҿ�ð�
		float timeTaken = timeInterval * i;
		//p = p0 + vt
		FVector prediction = player->leftHand->GetComponentLocation() + dir * timeTaken * range;
		//p.Z -= 0.5 * g * t^2
		prediction.Z += 0.5f * currentWorld->GetDefaultGravityZ() * timeTaken * timeTaken;
		////1-2. 0.1�ʾ� 0~50��° ������ ����ؼ� �迭�� �ִ´�
		lineLoc.Add(prediction);
		//����Ʈ���̽��� �������� üũ�Ѵ�
		FHitResult hitinfo;
		if (i > 0) {
			if (currentWorld->LineTraceSingleByChannel(hitinfo, lineLoc[i - 1], lineLoc[i], ECC_Visibility)) {
				//�ε��� ������ ������ ��ǥ�� �ְ� �ݺ����� ���� �����Ѵ�
				lineLoc.Add(hitinfo.ImpactPoint);
				break;
			}
		}
	}

	//2. �տ��� ���� ��ġ ���� ������ ������ �׸��� �׸��� (DrawLine�� �̿��Ѵ�)
	for (int32 i = 0; i < lineLoc.Num() - 1; i++) {
		DrawDebugLine(currentWorld, lineLoc[i], lineLoc[i + 1], FColor::Green, false, -1, 0, 2);
	}
}

void UMoveComponent::Teleport() {
	//����Ų �������� �����̵� �Ѵ� (�׽� ��ǥ �̵�)
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