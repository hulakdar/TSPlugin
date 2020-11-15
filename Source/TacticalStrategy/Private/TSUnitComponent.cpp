// Fill out your copyright notice in the Description page of Project Settings.


#include "TSUnitComponent.h"

// Sets default values for this component's properties
UTSUnitComponent::UTSUnitComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UTSUnitComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UTSUnitComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

