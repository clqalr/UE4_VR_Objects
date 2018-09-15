// Fill out your copyright notice in the Description page of Project Settings.

#include "VRDoor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "Engine/World.h"
#include "MotionControllerComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AVRDoor::AVRDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bOverLapped = false;
	bIsFirstInstance = true;
	bIsOpened = false;
	bFrontKnobOverlapped = false;
	bRearKnobOverlapped = false;
	
	MainMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("MainMesh"));

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(FName("BoxCollision"));
	CollisionBox->SetupAttachment(MainMesh);

	CollisionBox2 = CreateDefaultSubobject<UBoxComponent>(FName("BoxCollision2"));
	CollisionBox2->SetupAttachment(MainMesh);

	Root = CreateDefaultSubobject<USceneComponent>(FName("Scene"));
	Root->SetupAttachment(GetRootComponent());

	AttachedControllerMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("ControllerMesh"));
	AttachedControllerMesh->SetupAttachment(Root);

	AttachedControllerMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(FName("ControllerMesh2"));
	AttachedControllerMesh2->SetupAttachment(Root);

	bIsGripped = false;
	
}

// Called when the game starts or when spawned
void AVRDoor::BeginPlay()
{
	Super::BeginPlay();

	
	RotateValue = 1.0f;

	// if some door curve is set in blueprint
	if (DoorCurve)
	{
		FOnTimelineFloat TimelineCallback;
		FOnTimelineEventStatic TimelineFinishedCallback;
		TimelineFinishedCallback.BindUFunction(this, FName{ TEXT("SlamSoundCue") });
		TimelineCallback.BindUFunction(this, FName("ControlDoor"));
		MyTimeLine.AddInterpFloat(DoorCurve, TimelineCallback);
		MyTimeLine.SetTimelineFinishedFunc(TimelineFinishedCallback);
	}

	UE_LOG(LogTemp, Warning, TEXT("CHARACTER YAW2: %f"), GetActorRotation().Yaw);

	OnActorBeginOverlap.AddDynamic(this, &AVRDoor::OnBeginHandOverlap);
	OnActorEndOverlap.AddDynamic(this, &AVRDoor::OnEndHandOverlap);

	EnableInput(GetWorld()->GetFirstPlayerController());
	

	UInputComponent* InputComp = this->InputComponent;

	if (InputComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("Inputcomponent"));
		CustomSetupPlayerInputComponent(InputComp);
	}

}

// Called every frame
void AVRDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	MyTimeLine.TickTimeline(DeltaTime);

	// display current states every tick
	UE_LOG(LogTemp, Warning, TEXT("bIsGripped: %s"), (bIsGripped ? TEXT("True") : TEXT("False")));
	UE_LOG(LogTemp, Warning, TEXT("bIsFirstInstance: %s"), (bIsFirstInstance ? TEXT("True") : TEXT("False")));
	UE_LOG(LogTemp, Warning, TEXT("bIsOpened: %s"), (bIsOpened ? TEXT("True") : TEXT("False")));
	UE_LOG(LogTemp, Warning, TEXT("bOverLapped: %s"), (bOverLapped ? TEXT("True") : TEXT("False")));
	UE_LOG(LogTemp, Warning, TEXT("currYaw: %f"), GetActorRotation().Yaw);

	if (bIsFirstInstance && bIsGripped)
	{
		InitialPos = ControllerActor->GetActorLocation();
		InitialRot = GetActorRotation();
		bIsFirstInstance = false;
	}

	float Offset = 0.f;

	float Ang;
	float rad;

	// ControllerActor is null when the door knob and controller haven't overlapped
	if (ControllerActor != nullptr)
	{
		// Get a vector from origin to the controller's position
		FVector TempVec = ControllerActor->GetActorLocation() - GetActorLocation();

		// Use Atan2 to calculate the degree between the origin and the controller
		rad = FMath::Atan2(TempVec.Y, TempVec.X);
		Ang = FMath::RadiansToDegrees(rad);

		// Normalize the degree in 0 - 360 
		Ang = FMath::Fmod(Ang + 360.f, 360.f);
	}

	// Calculate offset
	if (!bIsFirstInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("AngAng: %f"), Ang);
		float CurrRotation = FMath::Fmod(GetActorRotation().Yaw + 360.f, 360.f);
		UE_LOG(LogTemp, Warning, TEXT("CurrRotation: %f"), CurrRotation);

		UE_LOG(LogTemp, Warning, TEXT("GetActorRotation().Yaw + YOffset: %f"), GetActorRotation().Yaw + Offset, 0.f);
		UE_LOG(LogTemp, Warning, TEXT("GetActorRotation().Yaw : %f"), GetActorRotation().Yaw);

		// add offset relative to the doors origin
		
		UE_LOG(LogTemp, Warning, TEXT("Angle Between Door and Controller : %f"), Ang);

		UE_LOG(LogTemp, Warning, TEXT("1Offset : %f"), 90.f + Ang);
		
		SetActorRotation(FRotator(0.f, Ang + 90.f, 0.f));
	}			

	if (ControllerActor != nullptr)
	{
		LastControllerPos = GetActorLocation();
	}
}

void AVRDoor::ControlDoor()
{
	TimelineValue = MyTimeLine.GetPlaybackPosition();
	CurveFloatValue = RotateValue * DoorCurve->GetFloatValue(TimelineValue);
	FQuat NewRotation = FQuat(FRotator(0.f, CurveFloatValue, 0.f));
	MainMesh->SetRelativeRotation(NewRotation);
}

void AVRDoor::ToggleDoor()
{
	MyTimeLine.PlayFromStart();
}


void AVRDoor::SlamSoundCue()
{
	MainMesh->SetSimulatePhysics(false);
}

void AVRDoor::OnBeginHandOverlap(AActor * OverlappedActor, AActor * OtherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlapped"));

	if (OtherActor->ActorHasTag(TEXT("MotionController")))
	{
		UE_LOG(LogTemp, Warning, TEXT("Motion Controller Overlapped1"));
	}


	TArray<UBoxComponent *> Comps;

	OverlappedActor->GetComponents(Comps);

	// Detects Front Knob and Rear Knob
	for (UBoxComponent * Knob : Comps)
	{
		if (Knob->ComponentHasTag(TEXT("FrontKnob"))) 
		{
			bFrontKnobOverlapped = true;
		}
		else if (Knob->ComponentHasTag(TEXT("RearKnob")))
		{
			UE_LOG(LogTemp, Warning, TEXT("Rear"));
			bRearKnobOverlapped = true;
		}
	}

	TArray<AActor*> OverlappedActors;
	GetOverlappingActors(OverlappedActors);

	for (AActor * OverlappingActor : OverlappedActors)
	{
		if (OverlappingActor->ActorHasTag(TEXT("MotionController")))
		{
			UE_LOG(LogTemp, Warning, TEXT("Overlapped"));
			bOverLapped = true;
			ControllerActor = OtherActor;
		}
	}
	
}

void AVRDoor::OnEndHandOverlap(AActor * OverlappedActor, AActor * OtherActor)
{
	if (OtherActor->ActorHasTag(TEXT("MotionController")))
	{
		UE_LOG(LogTemp, Warning, TEXT("Out of Overlap1"));
		bOverLapped = false;
		bFrontKnobOverlapped = false;
		bRearKnobOverlapped = false;
	}
}

void AVRDoor::CustomSetupPlayerInputComponent(UInputComponent* CustomInputComponent)
{
	CustomInputComponent->bBlockInput = false;

	FInputActionBinding &Binding = CustomInputComponent->BindAction("Trigger", IE_Pressed, this, &AVRDoor::GripPressed);
	FInputActionBinding &Binding2 = CustomInputComponent->BindAction("Trigger", IE_Released, this, &AVRDoor::GripReleased);
	
	Binding.bConsumeInput = false;
	Binding2.bConsumeInput = false;
}

void AVRDoor::GripPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("Grip Pressed"));

	if (bOverLapped)
	{
		bIsGripped = true;

		ControllerActor->SetActorHiddenInGame(true);
		AttachedControllerMesh->SetVisibility(true);
	}
}

void AVRDoor::GripReleased()
{
	if (ControllerActor != nullptr && bIsGripped)
	{
		bIsGripped = false;
		bIsFirstInstance = true;
		ControllerActor->SetActorHiddenInGame(false);
		AttachedControllerMesh->SetVisibility(false);

		TArray<USphereComponent*> Components;
		ControllerActor->GetComponents<USphereComponent>(Components);

		for (USphereComponent * Component : Components)
		{
			if (Component != nullptr)
			{
				// This will not work
				//Component->SetSimulatePhysics(true);

				// Set Physics (Linear) Velocity to this door from motion controller 
				FVector LinVel = Component->GetPhysicsLinearVelocity();
				MainMesh->SetPhysicsLinearVelocity(LinVel);
			}
		}
	}
}
