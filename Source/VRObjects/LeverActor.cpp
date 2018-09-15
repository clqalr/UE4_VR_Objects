// Fill out your copyright notice in the Description page of Project Settings.

#include "LeverActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"

// Sets default values
ALeverActor::ALeverActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	LeverMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Lever"));

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(FName("Knob"));
	CollisionBox->SetupAttachment(LeverMesh);

	ControllerActor = nullptr;

	bIsGripped = false;
	bIsFirstInstance = true;
	bOverlapped = false;
	bIsPulling = false;
}

// Called when the game starts or when spawned
void ALeverActor::BeginPlay()
{
	Super::BeginPlay();
	
	EnableInput(GetWorld()->GetFirstPlayerController());

	UInputComponent * InputComp = InputComponent;

	if (InputComp)
	{
		CustomSetupPlayerInputComponent(InputComp);
	}

	OnActorBeginOverlap.AddDynamic(this, &ALeverActor::OnBeginHandOverlap);
	OnActorEndOverlap.AddDynamic(this, &ALeverActor::OnEndHandOverlap);

	LastControllerPos.X = 0.f;
	LastControllerPos.Y = 0.f;
	LastControllerPos.Z = 0.f;
}



// Called every frame
void ALeverActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FRotator Rot = GetActorRotation();


	if (bIsGripped)
	{

		float offset;
		FVector Vec = ControllerActor->GetActorLocation() - LastContPos;
		UE_LOG(LogTemp, Warning, TEXT("vv1: %f"), ControllerActor->GetActorLocation().X);
		UE_LOG(LogTemp, Warning, TEXT("vv2: %f"), LastControllerPos.X);
		Vec.Normalize();

		float Val = FVector::DotProduct(GetActorForwardVector(), Vec);
		UE_LOG(LogTemp, Warning, TEXT("Val: %f"), Val);
		FVector DistVec;

	
		if (Val < 0.f)
		{
			UE_LOG(LogTemp, Warning, TEXT("Push"));

			// if user switched direction, reset
			if (!bIsPulling)
			{
				LastControllerPos = ControllerActor->GetActorLocation();

				LastPos = GetActorRotation();
			}

			DistVec = ControllerActor->GetActorLocation() - LastControllerPos;
			offset = DistVec.Size2D();
			UE_LOG(LogTemp, Warning, TEXT("sex: %f"), DistVec.X);

			Rot.Pitch = LastPos.Pitch + offset;

			bIsPulling = true;
		}
		else if (Val > 0.0004f)
		{
			UE_LOG(LogTemp, Warning, TEXT("Pull"));

			// if user switched direction, reset
			if (bIsPulling)
			{
				LastControllerPos = ControllerActor->GetActorLocation();

				LastPos = GetActorRotation();
			}

			DistVec = ControllerActor->GetActorLocation() - LastControllerPos;
			offset = DistVec.Size2D();
			UE_LOG(LogTemp, Warning, TEXT("sex: %f"), DistVec.X);
			Rot.Pitch = LastPos.Pitch - offset;

			bIsPulling = false;
		}


		UE_LOG(LogTemp, Warning, TEXT("LastPos.Pitch: %f"), LastPos.Pitch);

		UE_LOG(LogTemp, Warning, TEXT("Rottemn: %f"), Rot.Pitch);

		// Limit drawer movement
		if (Rot.Pitch >= 45.f)
		{
			Rot.Pitch = 45.f;
		}
		else if (Rot.Pitch <= -45.f)
		{
			Rot.Pitch = -45.f;
		}
	

		SetActorRelativeRotation(Rot);
		LastContPos = ControllerActor->GetActorLocation();
	}

}

void ALeverActor::CustomSetupPlayerInputComponent(UInputComponent * CustomInputComponent)
{
	FInputActionBinding &Binding = CustomInputComponent->BindAction("Trigger", 
																	IE_Pressed, this, 
																	&ALeverActor::GripPressed);

	FInputActionBinding &Binding2 = CustomInputComponent->BindAction("Trigger", 
																	 IE_Released, this, 
																	 &ALeverActor::GripReleased);

	// Let other actors to use input
	Binding.bConsumeInput = false;
	Binding2.bConsumeInput = false;
}


void ALeverActor::OnBeginHandOverlap(AActor * OverlappedActor, AActor * OtherActor)
{

	TArray<AActor*> OverlappedActors;
	GetOverlappingActors(OverlappedActors);

	for (AActor * OverlappingActor : OverlappedActors)
	{
		if (OverlappingActor->ActorHasTag(TEXT("MotionController")))
		{
			bOverlapped = true;
			ControllerActor = OtherActor;
		}
	}
}

void ALeverActor::OnEndHandOverlap(AActor * OverlappedActor, AActor * OtherActor)
{
	TArray<UBoxComponent *> Comps;

	OverlappedActor->GetComponents(Comps);

	// Detect a Knob
	for (UBoxComponent * Knob : Comps)
	{
		if (Knob->ComponentHasTag(TEXT("Knob")))
		{
			UE_LOG(LogTemp, Warning, TEXT("OASIS: ENDOVERLAP"));
			bOverlapped = false;
			break;
		}

	}
}

void ALeverActor::GripPressed()
{
	if (bOverlapped)
	{
		bIsGripped = true;
		if (bIsFirstInstance)
		{
			LastControllerPos = ControllerActor->GetActorLocation();

			LastPos = GetActorRotation();
		}
		
		bIsFirstInstance = false;
	}
}

void ALeverActor::GripReleased()
{
	if (ControllerActor != nullptr && bIsGripped)
	{
		bIsGripped = false;
		bIsFirstInstance = true;
	}
}