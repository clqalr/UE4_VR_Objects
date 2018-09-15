// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LeverActor.generated.h"

UCLASS()
class VROBJECTS_API ALeverActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALeverActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent * LeverMesh;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent * CollisionBox;

	class AActor * ControllerActor;

	UFUNCTION()
	void OnBeginHandOverlap(AActor * OverlappedActor, AActor * OtherActor);

	UFUNCTION()
	void OnEndHandOverlap(AActor * OverlappedActor, AActor * OtherActor);
	
	UFUNCTION()
	void CustomSetupPlayerInputComponent(UInputComponent* CustomInputComponent);
	
	void GripPressed();

	void GripReleased();

	/* Status */
	bool bIsGripped;
	bool bIsFirstInstance;
	bool bOverlapped;
	bool bIsPulling;


	/* Values */
	FVector LastControllerPos;
	FRotator LastPos;
	FVector LastContPos;
};
