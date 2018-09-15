// Fill out your copyright notice in the Description page of Project Settings.

#include "Drawer.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/World.h"

// Sets default values
ADrawer::ADrawer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(FName("Root"));
	SetRootComponent(Root);

	Drawer = CreateDefaultSubobject<UStaticMeshComponent>(FName("Drawer"));
	Drawer->SetupAttachment(Root);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(FName("Knob"));
	CollisionBox->SetupAttachment(Drawer);



	bOverlapped = false;
	bIsFirstInstance = true;

	LastDotProductValue = -999.f;
	LastDist = 0.f;

	bIsPulling = false;

	NumDrawers = 0;
}

// Called when the game starts or when spawned
void ADrawer::BeginPlay()
{
	Super::BeginPlay();

	EnableInput(GetWorld()->GetFirstPlayerController());

	UInputComponent* InputComp = this->InputComponent;

	if (InputComp)
	{
		CustomSetupPlayerInputComponent(InputComp);
	}

	OnActorBeginOverlap.AddDynamic(this, &ADrawer::OnBeginHandOverlap);
	OnActorEndOverlap.AddDynamic(this, &ADrawer::OnEndHandOverlap);
	
}

// Called every frame
void ADrawer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector ForwardVector;
	ForwardVector = Drawer->GetForwardVector();

	FVector Loc = Drawer->RelativeLocation;
	UE_LOG(LogTemp, Warning, TEXT("ff x: %f"), ForwardVector.X);
	UE_LOG(LogTemp, Warning, TEXT("ff y: %f"), ForwardVector.Y);
	UE_LOG(LogTemp, Warning, TEXT("ff z: %f"), ForwardVector.Z);
	
	if (bGripped)
	{
		float offset;
		// Get velocity vector of a controller
		FVector VelocityVec = ControllerActor->GetActorLocation() - OldPos;
		//VelocityVec *= 3.f;

		// Determine if drawer is being pushed or pulled
		float val = FVector::DotProduct(ControllerActor->GetActorForwardVector(), VelocityVec);

		FVector DistVec;

		// Taking care of noise; ignore value between 0 < n > 0.1
		if (val < 0.f)
		{
			// if user switched direction, reset
			if (!bIsPulling)
			{
				LastContPos = ControllerActor->GetActorLocation();
				LastContPos.Z = 0.f;

				LastCollisionBoxPos = CollisionBox->GetComponentLocation();
				LastCollisionBoxPos.Z = CollisionBox->GetComponentLocation().Z;


				LastPos = Drawer->RelativeLocation;
				LastPos.Z = Drawer->RelativeLocation.Z;

			}
			DistVec = ControllerActor->GetActorLocation() - LastContPos;
			offset = DistVec.Size2D();

			UE_LOG(LogTemp, Warning, TEXT("PULLING"));
			Loc.X = LastPos.X + offset;
			bIsPulling = true;
		}
		else if (val > 0.1f)
		{
			// if user switched direction, reset
			if (bIsPulling)
			{
				LastContPos = ControllerActor->GetActorLocation();
				LastContPos.Z = 0.f;

				LastCollisionBoxPos = CollisionBox->GetComponentLocation();
				LastCollisionBoxPos.Z = CollisionBox->GetComponentLocation().Z;

				LastPos = Drawer->RelativeLocation;
				LastPos.Z = Drawer->RelativeLocation.Z;
			}

			DistVec = ControllerActor->GetActorLocation() - LastContPos;
			offset = DistVec.Size2D();

			UE_LOG(LogTemp, Warning, TEXT("PUSHING"));
			Loc.X = LastPos.X - offset;
			bIsPulling = false;
		}

		Loc.Z = Drawer->RelativeLocation.Z;

		// Limit drawer movement
		if (Loc.X >= 60.f)
		{
			Loc.X = 60.f;
		}
		else if (Loc.X <= 0.f)
		{
			Loc.X = 0.f;
		}

		Drawer->SetRelativeLocation(Loc);

		LastDotProductValue = val;

		LastPosDrawer = Drawer->RelativeLocation;
		OldPos = ControllerActor->GetActorLocation();
		LastOffset = offset;
	}
	
}

void ADrawer::OnBeginHandOverlap(AActor * OverlappedActor, AActor * OtherActor)
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

void ADrawer::OnEndHandOverlap(AActor * OverlappedActor, AActor * OtherActor)
{
	TArray<UBoxComponent *> Comps;

	OverlappedActor->GetComponents(Comps);

	// Detect a Knob
	for (UBoxComponent * Knob : Comps)
	{
		if (Knob->ComponentHasTag(TEXT("Knob")))
		{
			bOverlapped = false;
			break;
		}

	}
}

void ADrawer::CustomSetupPlayerInputComponent(UInputComponent * CustomInputComponent)
{
	CustomInputComponent->bBlockInput = false;

	FInputActionBinding &Binding = CustomInputComponent->BindAction("Trigger", IE_Pressed, this, &ADrawer::GripPressed);
	FInputActionBinding &Binding2 = CustomInputComponent->BindAction("Trigger", IE_Released, this, &ADrawer::GripReleased);

	// Let other actors to use input
	Binding.bConsumeInput = false;
	Binding2.bConsumeInput = false;
}

void ADrawer::GripPressed()
{
	if (bOverlapped)
	{
		bGripped = true;

		if (bIsFirstInstance)
		{

			LastContPos = ControllerActor->GetActorLocation();
			LastContPos.Z = 0.f;

		
			LastCollisionBoxPos = CollisionBox->GetComponentLocation();
			LastCollisionBoxPos.Z = CollisionBox->GetComponentLocation().Z;
		

			LastPos = Drawer->RelativeLocation;
			LastPos.Z = Drawer->RelativeLocation.Z;
	
		}
		bIsFirstInstance = false;
		//LastPosDrawer = Drawer->RelativeLocation;
	}
}

void ADrawer::GripReleased()
{
	if (ControllerActor != nullptr && bGripped)
	{
		bGripped = false;
		bIsFirstInstance = true;
		DrawerTag = FName();
	}
}

