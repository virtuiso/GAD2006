// Fill out your copyright notice in the Description page of Project Settings.


#include "TileGameManager.h"

#include "TilePlayerController.h"

// Sets default values
ATileGameManager::ATileGameManager() :
	GridSize(100),
	GridOffset(0, 0, 0.5f),
	MapExtendsInGrids(25),
	BrushRotation(0)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	GridSelection = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GridMesh"));
	GridSelection->SetupAttachment(RootComponent);

	Display = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Display"));
	Display->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMesh(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> GridMaterial(TEXT("Material'/Game/UI/MAT_GridSlot.MAT_GridSlot'"));

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> DisplayMaterial(TEXT("Material'/Game/Materials/M_Atlas.M_Atlas'"));

	GridSelection->SetStaticMesh(PlaneMesh.Object);
	GridSelection->SetMaterial(0, GridMaterial.Object);
	GridSelection->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Display->SetStaticMesh(PlaneMesh.Object);
	Display->SetMaterial(0, DisplayMaterial.Object);
	Display->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ATileGameManager::BeginPlay()
{
	Super::BeginPlay();

	if (auto PlayerController = Cast<ATilePlayerController>(GWorld->GetFirstPlayerController()))
	{
		PlayerController->GameManager = this;
	}

	if (TileTypes.Num() >= 0)
	{
		CurrentTileIndex = 0;
		TileHasChanged();
	}

	Display->SetRelativeScale3D(FVector(0.083f, 0.083f, 0.1f));
}



void ATileGameManager::OnActorInteraction(AActor* HitActor, FVector& Location, bool IsPressed)
{
	//No tile types?
	if (TileTypes.Num() == 0) return;

	FVector GridLoc = GridOffset;
	GridLoc.X = FMath::GridSnap(Location.X, GridSize);
	GridLoc.Y = FMath::GridSnap(Location.Y, GridSize);
	GridLoc.Z = Location.Z;

	UPlayerInput* Input = GWorld->GetFirstPlayerController()->PlayerInput;

	if (Input->WasJustPressed(EKeys::LeftMouseButton))
	{
		int GridX = GridLoc.X / GridSize + MapExtendsInGrids;
		int GridY = GridLoc.Y / GridSize + MapExtendsInGrids;

		if (GridX < 0 || GridY < 0 || GridX >= MapExtendsInGrids * 2 || GridY >= MapExtendsInGrids * 2)
		{
			//Can not place ot of the grid
			return;
		}

		//Already a tile here?
		if (Map[GridX][GridY] != nullptr) return;

		if (TileTypes.IsValidIndex(CurrentTileIndex))
		{
			ATileBase* SelectedTile = TileTypes[CurrentTileIndex];
			Map[GridX][GridY] = SelectedTile;

			FTransform TileTransform(GridLoc + GridOffset);
			TileTransform = FTransform(FRotator(0, BrushRotation, 0), TileTransform.GetLocation(), TileTransform.GetScale3D());
			SelectedTile->InstancedMesh->AddInstance(SelectedTile->InstancedMesh->GetRelativeTransform() * TileTransform, true);
		}

		UE_LOG(LogTemp, Warning, TEXT("Hit: %s - %f, %f, %f"), HitActor ? *HitActor->GetActorLabel() : TEXT("None"), Location.X, Location.Y, Location.Z);
	}
	else if (Input->WasJustPressed(EKeys::MouseScrollDown))
	{
		CurrentTileIndex = (CurrentTileIndex + 1) % TileTypes.Num();

		TileHasChanged();
	}
	else if (Input->WasJustPressed(EKeys::MouseScrollUp))
	{
		if (CurrentTileIndex - 1 < 0) CurrentTileIndex = TileTypes.Num() - 1;
		else CurrentTileIndex--;

		TileHasChanged();
	}
	else if (Input->WasJustPressed(EKeys::RightMouseButton))
	{
		BrushRotation = (BrushRotation + 90) % 360;
		SetActorRotation(FRotator(0, BrushRotation, 0));
	}
	else
	{
		GridSelection->SetWorldLocation(GridLoc + GridOffset);
		Display->SetWorldLocation(GridLoc + GridOffset);
	}
}
void ATileGameManager::TileHasChanged()
{
	UE_LOG(LogTemp, Warning, TEXT("TileType: %s"), *TileTypes[CurrentTileIndex]->GetActorLabel());
	UStaticMesh* CurrentMesh = TileTypes[CurrentTileIndex]->InstancedMesh->GetStaticMesh();
	Display->SetStaticMesh(CurrentMesh);
}