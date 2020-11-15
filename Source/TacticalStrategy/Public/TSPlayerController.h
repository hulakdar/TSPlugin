// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TSPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALSTRATEGY_API ATSPlayerController : public APlayerController
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
};
