// Fill out your copyright notice in the Description page of Project Settings.

#include "HandController.h"
#include "MotionControllerComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AHandController::AHandController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionController"));
	SetRootComponent(MotionController);
	MotionController->ComponentTags.Add(TEXT("MotionController"));
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	Sphere->SetupAttachment(MotionController);
	Sphere->SetSphereRadius(10.f);


	bOverLapped = false;

}

void AHandController::SetHand(EControllerHand hand)
{
	MotionController->SetTrackingSource(hand);
}

bool AHandController::isHolding()
{
	TArray<AActor *> Attachments;
	GetAttachedActors(Attachments);
	if (Attachments.Num() > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}


// Called when the game starts or when spawned
void AHandController::BeginPlay()
{
	Super::BeginPlay();

	// Delegates
	OnActorBeginOverlap.AddDynamic(this, &AHandController::OnBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &AHandController::OnEndOverlap);
}

// Called every frame
void AHandController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (bOverLapped)
	{
		//FVector Location = Actor->GetActorLocation();
		//Location.Z = MotionController->GetComponentLocation().Z;
		//Actor->SetActorLocation(Location);

	}

}

void AHandController::OnBeginOverlap(AActor * OverlappedActor, AActor * OtherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Controller Overlapped"));


}

void AHandController::OnEndOverlap(AActor * OverlappedActor, AActor * OtherActor)
{

}


