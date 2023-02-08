// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KeyboardActor.generated.h"

UCLASS()
class VR_51_API AKeyboardActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AKeyboardActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBoxComponent* compBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* compMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundBase* keySound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLinearColor keyColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float glowPower;

private:
	class UMaterialInstanceDynamic* keyMat;
	FTimerHandle colorHandle;

	void ChangeKeyColor(FVector4 color, float glow);
	void ColorOff();
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
