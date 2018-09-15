// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "VRCharacter.generated.h"


UCLASS()
class VROBJECTS_API AVRCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVRCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	class UCameraComponent * GetCameraComponent();

	bool IsGrippedPressed();
	bool IsGrippedReleased();

private:
	void MoveForward(float Amount);
	void MoveRight(float Amount);
		
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent * Camera;

	UPROPERTY(VisibleAnywhere)
	class USceneComponent * Scene;

	UPROPERTY(VisibleAnywhere)
	class USceneComponent * Scene2;

	UPROPERTY(VisibleAnywhere)
	class AHandController * RightController;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AHandController> HandControllerClass;


};
