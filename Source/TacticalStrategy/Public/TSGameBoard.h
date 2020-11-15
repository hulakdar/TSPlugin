// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TSGameBoard.generated.h"

UENUM(BlueprintType)
enum class ETileShape : uint8
{
	Square,
	Hex
};

UCLASS(Blueprintable)
class TACTICALSTRATEGY_API ATSGameBoard : public AActor
{
	GENERATED_BODY()

public:	
	ATSGameBoard();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=GameBoard)
	class UStaticMesh* TileMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=GameBoard)
	FIntPoint BoardSize;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=GameBoard)
	ETileShape TileShape;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=GameBoard,AdvancedDisplay)
	FVector2D TileOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=GameBoard,AdvancedDisplay)
	FRotator TileRotation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=GameBoard,AdvancedDisplay)
	FVector TileScale;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void PostEditMove(bool bFinished);
	void UpdateBoardTiles();
#endif

	virtual void BeginPlay() override;
protected:

	UPROPERTY(BlueprintReadOnly, Category=GameBoard)
	TArray<class UTSUnitComponent*> Units;

	UPROPERTY(VisibleAnywhere, Category=GameBoard)
	UInstancedStaticMeshComponent *Tiles;

	UFUNCTION()
	void OnTileClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
