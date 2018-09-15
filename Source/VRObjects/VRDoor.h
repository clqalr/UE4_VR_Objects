// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "VRDoor.generated.h"


UCLASS()
class VROBJECTS_API AVRDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVRDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere)
	UCurveFloat * DoorCurve;

	UFUNCTION()
	void ControlDoor();

	UFUNCTION()
	void ToggleDoor();

	float RotateValue;
	float CurveFloatValue;
	float TimelineValue;
	FRotator DoorRotation;
	FTimeline MyTimeLine;
	
private:
	class USoundCue * propellerAudioCue;
	class UAudioComponent* propellerAudioComponent;

	float DoorLowerLimit;
	float DoorUpperLimit;

	UFUNCTION()
	void SlamSoundCue();


	FVector LastControllerPos;
	float lastYaw;
	FVector OverlappedLocation;
	FRotator OverlappedRotation;

	UFUNCTION()
	void OnBeginHandOverlap(AActor * OverlappedActor, AActor * OtherActor);
	UFUNCTION()
	void OnEndHandOverlap(AActor * OverlappedActor, AActor * OtherActor);
	
	FVector InitialPos;
	FRotator InitialRot;

	bool bIsOpened;

	bool bOverLapped;

	bool bIsFirstInstance;

	bool bIsGripped;

	bool bFrontKnobOverlapped;
	bool bRearKnobOverlapped;

	UPROPERTY(VisibleAnywhere)
	class USceneComponent * Root;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent * CollisionBox;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent * CollisionBox2;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent * MainMesh;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent * AttachedControllerMesh;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent * AttachedControllerMesh2;

	class AActor * ControllerActor;

	UFUNCTION()
	void CustomSetupPlayerInputComponent(UInputComponent* CustomInputComponent);

	void GripPressed();
	
	void GripReleased();
	
};
