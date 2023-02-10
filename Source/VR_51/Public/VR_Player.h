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

	//ī�޶� ������Ʈ
	UPROPERTY(EditAnywhere, Category = "VR_Settings | Compoenets")
		class UCameraComponent* compCamera;

	//������ ��Ʈ�ѷ�
	UPROPERTY(EditAnywhere, Category = "VR_Settings | Compoenets")
		class UMotionControllerComponent* rightController;
	//������ ���
	UPROPERTY(EditAnywhere, Category = "VR_Settings | Compoenets")
		class USkeletalMeshComponent* rightHand;
	//������ �Է� �α�
	UPROPERTY(EditAnywhere, Category = "VR_Settings | Compoenets")
		class UTextRenderComponent* rightLog;

	//�޼� ��Ʈ�ѷ�
	UPROPERTY(EditAnywhere, Category = "VR_Settings | Compoenets")
		class UMotionControllerComponent* leftController;
	//�޼� ���
	UPROPERTY(EditAnywhere, Category = "VR_Settings | Compoenets")
		class USkeletalMeshComponent* leftHand;
	//�޼� �Է� �α�
	UPROPERTY(EditAnywhere, Category = "VR_Settings | Compoenets")
		class UTextRenderComponent* leftLog;

	//�Ӹ� ���
	UPROPERTY(EditAnywhere, Category = "VR_Settings | Compoenets")
		class UStaticMeshComponent* headMesh;

	//ī�޶� ���� ��ġ ����
	UPROPERTY(EditAnywhere, Category = "VR_Settings | ModuleSetting")
		TEnumAsByte<enum EHMDTrackingOrigin::Type> trackOrigin;

	//Input�� ���� �� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR_Settings | Inputs")
		class UInputMappingContext* myMapping;

	//�޼� Inputs
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR_Settings | Inputs")
		TArray<class UInputAction*> leftInputs;
	//������ Inputs
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

	//�޼� �Է�
	void OnTriggerLeft(const struct FInputActionValue& value);
	void OnGripLeft(const struct FInputActionValue& value);
	void OnButtonXLeft(const struct FInputActionValue& value);
	void OnButtonYLeft(const struct FInputActionValue& value);
	void OnButtonMenuLeft(const struct FInputActionValue& value);

	//������ �Է�
	void OnTriggerRight(const struct FInputActionValue& value);
	void OnGripRight(const struct FInputActionValue& value);
	void OnButtonARight(const struct FInputActionValue& value);
	void OnButtonBRight(const struct FInputActionValue& value);
	void OnButtonMenuRight(const struct FInputActionValue& value);
};
