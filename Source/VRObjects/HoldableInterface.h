// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface.h"
#include "HoldableInterface.generated.h"

/**
 * 
 */
UINTERFACE(Blueprintable)
class UHoldableInterface : public UInterface
{
	GENERATED_BODY()
};

class VROBJECTS_API IHoldableInterface
{
	GENERATED_BODY()

public:
	virtual void EnablePhysics() = 0;
	virtual void DisablePhysics() = 0;
	virtual void SetLinearVelocity(FVector LinVel) = 0;
	virtual void SetAngularVelocity(FVector AngVel) = 0;
};
