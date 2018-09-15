// Fill out your copyright notice in the Description page of Project Settings.

#include "VRMesh.h"
#include "HandController.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"


// Sets default values
AVRMesh::AVRMesh()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	MainMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Main Mesh"));

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(FName("BoxCollision"));
	CollisionBox->SetupAttachment(MainMesh);

	MotionController = nullptr;

	bOverlapped = false;
	bIsGripped = false;
}

// Called when the game starts or when spawned
void AVRMesh::BeginPlay()
{
	Super::BeginPlay();
	

	EnableInput(GetWorld()->GetFirstPlayerController());

	UInputComponent* InputComp = this->InputComponent;

	if (InputComp)
	{
		CustomSetupPlayerInputComponent(InputComp);
	}

	OnActorBeginOverlap.AddDynamic(this, &AVRMesh::OnBeginHandOverlap);
	OnActorEndOverlap.AddDynamic(this, &AVRMesh::OnEndHandOverlap);
}

// Called every frame
void AVRMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bOverlapped)
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlapped"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Not Overlapped"));
	}

}


void AVRMesh::OnBeginHandOverlap(AActor * OverlappedActor, AActor * OtherActor)
{
	bOverlapped = true;
	UE_LOG(LogTemp, Warning, TEXT("HI"));
	TArray<AActor*> OverlappedActors;
	GetOverlappingActors(OverlappedActors);

	for (AActor * OverlappingActor : OverlappedActors)
	{
		if (OverlappingActor->ActorHasTag(TEXT("MotionController")))
		{
			bOverlapped = true;
			this->MotionController = OtherActor;
		}
	}
}

void AVRMesh::OnEndHandOverlap(AActor * OverlappedActor, AActor * OtherActor)
{
	if (OtherActor->ActorHasTag(TEXT("MotionController")))
	{
		bOverlapped = false;
	}
}

void AVRMesh::CustomSetupPlayerInputComponent(UInputComponent* CustomInputComponent)
{
	CustomInputComponent->bBlockInput = false;


	FInputActionBinding &Binding = CustomInputComponent->BindAction("Trigger", IE_Pressed, this, &AVRMesh::GripPressed);
	FInputActionBinding &Binding2 = CustomInputComponent->BindAction("Trigger", IE_Released, this, &AVRMesh::GripReleased);
	Binding.bConsumeInput = false;
	Binding2.bConsumeInput = false;
	
}


void AVRMesh::GripPressed()
{
	if (bOverlapped)
	{
		bIsGripped = true;
		MainMesh->SetSimulatePhysics(false);
		AHandController * MC = Cast<AHandController>(MotionController);

		// Pick up one object at a time
		if (!MC->isHolding())
		{
			AttachToActor(MotionController, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		}
		
	}
}

void AVRMesh::GripReleased()
{
	if (bIsGripped)
	{
		bIsGripped = false;
		MainMesh->SetSimulatePhysics(true);

		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}
}

