// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HandController.generated.h"


UCLASS()
class VROBJECTS_API AHandController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHandController();
	
	void SetHand(EControllerHand hand);

	bool isHolding();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
	class UMotionControllerComponent * MotionController;

	// Dummy sphere for physics calculation
	UPROPERTY(VisibleAnywhere)
	class USphereComponent * Sphere;

	UFUNCTION()
	void OnBeginOverlap(AActor * OverlappedActor, AActor * OtherActor);

	UFUNCTION()
	void OnEndOverlap(AActor * OverlappedActor, AActor * OtherActor);
	bool bOverLapped;
	class AActor * Actor;
};
