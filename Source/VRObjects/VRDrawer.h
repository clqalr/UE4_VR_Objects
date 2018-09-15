// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VRDrawer.generated.h"

UCLASS()
class VROBJECTS_API AVRDrawer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVRDrawer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent * MainBodyMesh;

	UPROPERTY(VisibleAnywhere)
	USceneComponent * Root;

	//UPROPERTY(VisibleAnywhere)
	//USceneComponent * DrawerRoot;

	float Threshold;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent * CollisionBox;

	UFUNCTION()
	void OnBeginHandOverlap(AActor * OverlappedActor, AActor * OtherActor);
	
	UFUNCTION()
	void OnEndHandOverlap(AActor * OverlappedActor, AActor * OtherActor);

	UFUNCTION()
	void CustomSetupPlayerInputComponent(UInputComponent* CustomInputComponent);

	void GripPressed();

	void GripReleased();

	bool bOverlapped;

	bool bGripped;

	bool bIsFirstInstance;

	bool bIsPulling;

	class AActor * ControllerActor;
	float LastDist;
	float LastDotProductValue;
	FVector LastPos;
	FVector LastPosDrawer;
	FVector LastContPos;
	FVector LastCollisionBoxPos;
	FVector OldPos;

	UPROPERTY(EditAnywhere)
	int NumDrawers;
	
	TArray<UStaticMeshComponent> Drawers;

	float LastOffset;

	FName DrawerTag;
	FName HandleTag;
};
