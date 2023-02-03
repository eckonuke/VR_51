// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GraspComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class VR_51_API UGraspComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGraspComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// Called to bind functionality to input
	void SetupPlayerInputComponent(class UEnhancedInputComponent* PlayerInputComponent);

	UPROPERTY(EditAnywhere, Category = Inputs)
		class UInputAction* grip_right;
	UPROPERTY(EditAnywhere, Category = Inputs)
		class UInputAction* trigger_right;
	UPROPERTY(EditAnywhere, Category = Inputs)
		class UInputAction* trigger_right_touch;
	UPROPERTY(EditAnywhere, Category = Inputs)
		class UInputAction* thumb_right;
	UPROPERTY(EditAnywhere, Category = Inputs)
		class UInputAction* thumb_right_touch;

	UPROPERTY(EditAnywhere, Category = Inputs)
	float grabDistance = 30.0f;

private:
	class AVR_Player* player;
	class UVRHandAnimInstance* rightHandAnim;

	void GripRightAction(const struct FInputActionValue& value);
	void TriggerRightAction(const struct FInputActionValue& value);
	void TrggerRightTouch();
	void TrggerRightTouchEnd();
	void ThumbRightTouch();
	void ThumbRightTouchEnd();
	void ResetRightFingers();
	void GrabObject(USkeletalMeshComponent* selectHand);
};
