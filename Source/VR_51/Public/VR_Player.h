// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <HeadMountedDisplayFunctionLibrary.h>
#include "GameFramework/Character.h"
#include "VR_Player.generated.h"

UCLASS()
class VR_51_API AVR_Player : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVR_Player();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//카메라 컴포넌트
	UPROPERTY(EditAnywhere, Category = "VR_Settings | Compoenets")
		class UCameraComponent* compCamera;

	//오른손 컨트롤러
	UPROPERTY(EditAnywhere, Category = "VR_Settings | Compoenets")
		class UMotionControllerComponent* rightController;
	//오른손 모양
	UPROPERTY(EditAnywhere, Category = "VR_Settings | Compoenets")
		class USkeletalMeshComponent* rightHand;
	//오른손 입력 로그
	UPROPERTY(EditAnywhere, Category = "VR_Settings | Compoenets")
		class UTextRenderComponent* rightLog;

	//왼손 컨트롤러
	UPROPERTY(EditAnywhere, Category = "VR_Settings | Compoenets")
		class UMotionControllerComponent* leftController;
	//왼손 모양
	UPROPERTY(EditAnywhere, Category = "VR_Settings | Compoenets")
		class USkeletalMeshComponent* leftHand;
	//왼손 입력 로그
	UPROPERTY(EditAnywhere, Category = "VR_Settings | Compoenets")
		class UTextRenderComponent* leftLog;

	//머리 모양
	UPROPERTY(EditAnywhere, Category = "VR_Settings | Compoenets")
		class UStaticMeshComponent* headMesh;

	//카메라 높이 위치 설정
	UPROPERTY(EditAnywhere, Category = "VR_Settings | ModuleSetting")
		TEnumAsByte<enum EHMDTrackingOrigin::Type> trackOrigin;

	//Input에 대한 총 맵핑
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR_Settings | Inputs")
		class UInputMappingContext* myMapping;

	//왼손 Inputs
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR_Settings | Inputs")
		TArray<class UInputAction*> leftInputs;
	//오른손 Inputs
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR_Settings | Inputs")
		TArray<class UInputAction*> rightInputs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR_Settings | Components")
		class UMoveComponent* moveComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR_Settings | Components")
		class UGraspComponent* graspComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR_Settings | Components")
		class UWidgetInteractionComponent* widgetPointer_right;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR_Settings | Components")
		class UWidgetPointerComponent* widgetPointerComp;

private:
	void Recenter();

	//왼손 입력
	void OnTriggerLeft(const struct FInputActionValue& value);
	void OnGripLeft(const struct FInputActionValue& value);
	void OnButtonXLeft(const struct FInputActionValue& value);
	void OnButtonYLeft(const struct FInputActionValue& value);
	void OnButtonMenuLeft(const struct FInputActionValue& value);

	//오른손 입력
	void OnTriggerRight(const struct FInputActionValue& value);
	void OnGripRight(const struct FInputActionValue& value);
	void OnButtonARight(const struct FInputActionValue& value);
	void OnButtonBRight(const struct FInputActionValue& value);
	void OnButtonMenuRight(const struct FInputActionValue& value);
};
