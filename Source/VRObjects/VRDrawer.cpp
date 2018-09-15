// Fill out your copyright notice in the Description page of Project Settings.

#include "VRDrawer.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/World.h"

// Sets default values
AVRDrawer::AVRDrawer()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(FName("Root"));
	SetRootComponent(Root);

	MainBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Main Body"));
	MainBodyMesh->SetupAttachment(Root);

	//DrawerRoot = CreateDefaultSubobject<USceneComponent>(FName("DrawerRoot"));
	//DrawerRoot->SetupAttachment(MainBodyMesh);

	


	Threshold = 60.f;

	bOverlapped = false;
	bIsFirstInstance = true;

	LastDotProductValue = -999.f;
	LastDist = 0.f;

	bIsPulling = false;

	NumDrawers = 0;

}

// Called when the game starts or when spawned
void AVRDrawer::BeginPlay()
{
	Super::BeginPlay();

	EnableInput(GetWorld()->GetFirstPlayerController());

	UInputComponent* InputComp = this->InputComponent;

	if (InputComp)
	{
		CustomSetupPlayerInputComponent(InputComp);
	}

	OnActorBeginOverlap.AddDynamic(this, &AVRDrawer::OnBeginHandOverlap);
	OnActorEndOverlap.AddDynamic(this, &AVRDrawer::OnEndHandOverlap);
}

// Called every frame
void AVRDrawer::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);
	

	

}

void AVRDrawer::CustomSetupPlayerInputComponent(UInputComponent* CustomInputComponent)
{
	CustomInputComponent->bBlockInput = false;

	FInputActionBinding &Binding = CustomInputComponent->BindAction("Trigger", IE_Pressed, this, &AVRDrawer::GripPressed);
	FInputActionBinding &Binding2 = CustomInputComponent->BindAction("Trigger", IE_Released, this, &AVRDrawer::GripReleased);

	// Let other actors to use input
	Binding.bConsumeInput = false;
	Binding2.bConsumeInput = false;
}

void AVRDrawer::GripPressed()
{
	
}

void AVRDrawer::GripReleased()
{

}

void AVRDrawer::OnBeginHandOverlap(AActor * OverlappedActor, AActor * OtherActor)
{
	
}

void AVRDrawer::OnEndHandOverlap(AActor * OverlappedActor, AActor * OtherActor)
{

}
