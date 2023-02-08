// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyboardActor.h"
#include <Components/BoxComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Materials/MaterialInterface.h>
#include <Materials/MaterialInstanceDynamic.h>

// Sets default values
AKeyboardActor::AKeyboardActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	compBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(compBox);
	compBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	compBox->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	compBox->SetGenerateOverlapEvents(true);
	compBox->SetBoxExtent(FVector(50));

	compMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	compMesh->SetupAttachment(compBox);
	compMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AKeyboardActor::BeginPlay() {
	Super::BeginPlay();
	compBox->OnComponentBeginOverlap.AddDynamic(this, &AKeyboardActor::OnBoxBeginOverlap);

	//다이나믹 머티리얼 인스턴스를 생성한다.
	keyMat = UMaterialInstanceDynamic::Create(compMesh->GetMaterial(0), this);
	compMesh->SetMaterial(0, keyMat);
}

// Called every frame
void AKeyboardActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}


void AKeyboardActor::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	UGameplayStatics::PlaySoundAtLocation(this, keySound, GetActorLocation(), 1, 0);
	//키의 색상을 녹색으로 빛나게 한다
	ChangeKeyColor(keyColor, glowPower);
	GetWorldTimerManager().ClearTimer(colorHandle);
	GetWorldTimerManager().SetTimer(colorHandle, this, &AKeyboardActor::ColorOff, 0.5f, false);
}

void AKeyboardActor::ChangeKeyColor(FVector4 color, float glow) {
	keyMat->SetVectorParameterValue(TEXT("EmissiveColor"), color);
	keyMat->SetScalarParameterValue(TEXT("Glow"), glow);
}

void AKeyboardActor::ColorOff() {
	ChangeKeyColor(FVector4(1, 1, 1, 1), 0);
}
