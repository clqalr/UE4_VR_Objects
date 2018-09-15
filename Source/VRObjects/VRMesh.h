// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VRMesh.generated.h"

UCLASS()
class VROBJECTS_API AVRMesh : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVRMesh();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
private:
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent * MainMesh;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent * CollisionBox;
	
	AActor * MotionController;

	UFUNCTION()
	void OnBeginHandOverlap(AActor * OverlappedActor, AActor * OtherActor);

	UFUNCTION()
	void OnEndHandOverlap(AActor * OverlappedActor, AActor * OtherActor);

	UFUNCTION()
	void CustomSetupPlayerInputComponent(class UInputComponent* CustomInputComponent);

	bool bOverlapped;
	bool bIsGripped;

	
	void GripPressed();
	void GripReleased();
};
