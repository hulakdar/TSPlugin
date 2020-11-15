// Fill out your copyright notice in the Description page of Project Settings.

#include "TSGameBoard.h"
#include "Components/BillboardComponent.h"
#include "Engine/CollisionProfile.h"

PRAGMA_DISABLE_OPTIMIZATION

// Sets default values
ATSGameBoard::ATSGameBoard()
{
	ensure(!TileMesh);

	static ConstructorHelpers::FObjectFinderOptional<UStaticMesh> DefaultTileMesh(TEXT("/TacticalStrategy/EditorResources/SM_DefaultTile"));
	TileMesh = DefaultTileMesh.Get();
	PrimaryActorTick.bCanEverTick = true;

	if (RootComponent == nullptr)
	{
		SetRootComponent(CreateDefaultSubobject<USceneComponent>("Root"));
	}

	Tiles = CreateDefaultSubobject<UInstancedStaticMeshComponent>("Tiles");
	Tiles->SetupAttachment(RootComponent);
	Tiles->SetStaticMesh(TileMesh);
	Tiles->NumCustomDataFloats = 1;

	BoardSize = FIntPoint(2, 2);
	TileScale = FVector(1.f);
}

#if WITH_EDITOR
void ATSGameBoard::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	FProperty* MemberPropertyThatChanged = PropertyChangedEvent.MemberProperty;
	const FName MemberPropertyName = MemberPropertyThatChanged != nullptr ? MemberPropertyThatChanged->GetFName() : NAME_None;

	if (MemberPropertyName == GET_MEMBER_NAME_CHECKED(ThisClass, BoardSize)
	||	MemberPropertyName == GET_MEMBER_NAME_CHECKED(ThisClass, TileMesh)
	||	MemberPropertyName == GET_MEMBER_NAME_CHECKED(ThisClass, TileOffset)
	||	MemberPropertyName == GET_MEMBER_NAME_CHECKED(ThisClass, TileRotation)
	||	MemberPropertyName == GET_MEMBER_NAME_CHECKED(ThisClass, TileShape)
	||	MemberPropertyName == GET_MEMBER_NAME_CHECKED(ThisClass, TileScale) )
	{
		Tiles->SetStaticMesh(TileMesh);
		UpdateBoardTiles();
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void ATSGameBoard::PostEditMove(bool bFinished)
{
	if (Tiles && TileMesh)
	{
		Tiles->SetStaticMesh(TileMesh);
		UpdateBoardTiles();
	}

	Super::PostEditMove(bFinished);
}

void ATSGameBoard::UpdateBoardTiles()
{
	if (!TileMesh)
	{
		return;
	}
	Tiles->ClearInstances();
	Tiles->PreAllocateInstancesMemory(BoardSize.X * BoardSize.Y);

	for (int i = 0; i < BoardSize.X; ++i)
	{
		for (int j = 0; j < BoardSize.Y; ++j)
		{
			bool ShouldOffset = TileShape == ETileShape::Hex && (j & 1);
			FVector2D BoardCenterOffset = FVector2D(BoardSize) / 2 - 0.5;
			FBoxSphereBounds MeshBounds = TileMesh->GetBounds();

			FVector InstancePosition =
				(FVector(i - ShouldOffset * 0.5, j, 0) - FVector(BoardCenterOffset, 0))
			*	(MeshBounds.BoxExtent * 2 + FVector(TileOffset, 0));

			FHitResult HitResult;
			FVector SweepOffset = MeshBounds.BoxExtent.ProjectOnToNormal(FVector::UpVector);
			float HalfHeight = 300;

			bool bSpotTaken = GetWorld()->OverlapAnyTestByChannel(
				GetActorTransform().TransformPosition(InstancePosition + FVector::UpVector * (HalfHeight + 2)),
				FQuat::Identity,
				ECollisionChannel::ECC_WorldStatic,
				FCollisionShape::MakeCapsule(MeshBounds.SphereRadius * 0.6, HalfHeight)
			);

			if (!bSpotTaken)
			{
				Tiles->AddInstance(FTransform(TileRotation, InstancePosition, TileScale));
			}
		}
	}
}
#endif

// Called when the game starts or when spawned
void ATSGameBoard::BeginPlay()
{
	Super::BeginPlay();
	
	Tiles->OnClicked.AddDynamic(this, &ATSGameBoard::OnTileClicked);
}

void ATSGameBoard::OnTileClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	APlayerController *PC = GetWorld()->GetFirstPlayerController();

	FHitResult HitResult;
	if (PC->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult)
		&& HitResult.Component == Tiles)
	{
		TArray<int32> Instances = Tiles->GetInstancesOverlappingSphere(HitResult.Location, 10);
		if (Instances.Num() == 1)
		{
			Tiles->SetCustomDataValue(Instances[0], 0, 10, true);
		}
	}
}

// Called every frame
void ATSGameBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATSGameBoard::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
}

PRAGMA_ENABLE_OPTIMIZATION
