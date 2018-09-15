// Fill out your copyright notice in the Description page of Project Settings.

#include "VRCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "MotionControllerComponent.h"
#include "Engine/World.h"
#include "HandController.h"



// Sets default values
AVRCharacter::AVRCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	//scene->SetRelativeLocation(FVector(0.f, 0.f, -110.f));
	Scene->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(Scene);

}

// Called when the game starts or when spawned
void AVRCharacter::BeginPlay()
{
	Super::BeginPlay();

	RightController = GetWorld()->SpawnActor<AHandController>(HandControllerClass);

	if (RightController != nullptr)
	{
		RightController->AttachToComponent(Scene, FAttachmentTransformRules::KeepRelativeTransform);
		RightController->SetHand(EControllerHand::Right);
	}	
}

// Called every frame
void AVRCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FVector NewCameraOffset = Camera->GetComponentLocation() - GetActorLocation();
	NewCameraOffset.Z = 0;
	AddActorWorldOffset(NewCameraOffset);
	Scene->AddWorldOffset(-NewCameraOffset);
}

void AVRCharacter::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AVRCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AVRCharacter::MoveRight);
}

/** Returns Camera Component
 *  @Return A Pointer to UCameraComponent
 */
UCameraComponent * AVRCharacter::GetCameraComponent()
{
	return Camera;
}



void AVRCharacter::MoveForward(float Amount)
{
	AddMovementInput(Amount * Camera->GetForwardVector());
}

void AVRCharacter::MoveRight(float Amount)
{
	AddMovementInput(Amount * Camera->GetRightVector());
}

