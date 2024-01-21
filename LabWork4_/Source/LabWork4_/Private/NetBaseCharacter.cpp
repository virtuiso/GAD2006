// Fill out your copyright notice in the Description page of Project Settings.


#include "NetBaseCharacter.h"
#include "Engine/DataTable.h"
#include "NetGameInstance.h"

static UDataTable* SBodyParts = nullptr;

static wchar_t* BodyPartNames[] =
{
	TEXT("Face"),
	TEXT("Hair"),
	TEXT("Chest"),
	TEXT("Hands"),
	TEXT("Legs"),
	TEXT("Beard"),
	TEXT("Eyebrows")
};

// Sets default values
ANetBaseCharacter::ANetBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PartFace = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Face"));
	PartFace->SetupAttachment(GetMesh());
	PartFace->SetLeaderPoseComponent(GetMesh());

	PartHands = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hands"));
	PartHands->SetupAttachment(GetMesh());
	PartHands->SetLeaderPoseComponent(GetMesh());

	PartLegs = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Legs"));
	PartLegs->SetupAttachment(GetMesh());
	PartLegs->SetLeaderPoseComponent(GetMesh());

	PartHair = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hair"));
	PartHair->SetupAttachment(PartFace, FName("headSocket"));

	PartBeard = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Beard"));
	PartBeard->SetupAttachment(PartFace, FName("headSocket"));

	PartEyebrows = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Eyebrows"));
	PartEyebrows->SetupAttachment(PartFace, FName("headSocket"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> 
		SK_Eyes(TEXT("StaticMesh'/Game/StylizedModularChar/Meshes/SM_Eyes.SM_Eyes'"));

	PartEyes = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Eyes"));
	PartEyes->SetupAttachment(PartFace, FName("headSocket"));
	PartEyes->SetStaticMesh(SK_Eyes.Object);

	static ConstructorHelpers::FObjectFinder<UDataTable>
		DT_BodyParts(TEXT("DataTable'/Game/Blueprints/DT_DataTable.DT_DataTable'"));
	SBodyParts = DT_BodyParts.Object;

}

// Called when the game starts or when spawned
void ANetBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocallyControlled())
	{
		UNetGameInstance* Instance = Cast<UNetGameInstance>(GWorld->GetGameInstance());
		if (Instance && Instance->PlayerInfo.Ready) {
			SubmitPlayerInfoToServer(Instance->PlayerInfo);
		}
	}
	
}

void ANetBaseCharacter::OnConstruction(const FTransform& Transform)
{
	UpdateBodyParts();
}

// Called every frame
void ANetBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANetBaseCharacter::ChangeBodyPart(EBodyPart index, int value, bool DirectSet)
{
	FSMeshAssetList* List = GetBodyPartList(index, PartSelection.isFemale);
	if (List == nullptr) return;

	int CurrentIndex = PartSelection.Indices[(int)index];

	if (DirectSet)
	{
		CurrentIndex = value;
	}
	else {
		CurrentIndex += value;
	}

	int Num = List->ListSkeletal.Num() + List->ListStatic.Num();

	if (CurrentIndex < 0)
		CurrentIndex += Num;
	else
		CurrentIndex %= Num;

	PartSelection.Indices[(int)index] = CurrentIndex;

	switch (index)
	{
		case EBodyPart::BP_FACE:
			PartFace->SetSkeletalMeshAsset(List->ListSkeletal[CurrentIndex]);
			break;

		case EBodyPart::BP_BEARD:
			PartBeard->SetStaticMesh(List->ListStatic[CurrentIndex]);
			break;

		case EBodyPart::BP_CHEST:
			GetMesh()->SetSkeletalMeshAsset(List->ListSkeletal[CurrentIndex]);
			break;

		case EBodyPart::BP_HAIR:
			PartHair->SetStaticMesh(List->ListStatic[CurrentIndex]);
			break;

		case EBodyPart::BP_HANDS:
			PartHands->SetSkeletalMeshAsset(List->ListSkeletal[CurrentIndex]);
			break;

		case EBodyPart::BP_LEGS:
			PartLegs->SetSkeletalMeshAsset(List->ListSkeletal[CurrentIndex]);
			break;

		case EBodyPart::BP_EYEBROWS:
			PartEyebrows->SetStaticMesh(List->ListStatic[CurrentIndex]);
			break;

	}
}

void ANetBaseCharacter::ChangeGender(bool _isFemale)
{
	PartSelection.isFemale = _isFemale;
	UpdateBodyParts();
}

void ANetBaseCharacter::SubmitPlayerInfoToServer_Implementation(FSPlayerInfo Info)
{
	PartSelection = Info.BodyParts;

	if (HasAuthority())
	{
		OnRep_PlayerInfoChanged();
	}
}

void ANetBaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ANetBaseCharacter, PartSelection);
}

void ANetBaseCharacter::OnRep_PlayerInfoChanged()
{
	UpdateBodyParts();
}

FSMeshAssetList* ANetBaseCharacter::GetBodyPartList(EBodyPart part, bool isFemale)
{
	FString Name = FString::Printf(TEXT("%s%s"), isFemale ? TEXT("Female") : TEXT("Male"), BodyPartNames[(int)part]);
	return SBodyParts ? SBodyParts->FindRow<FSMeshAssetList>(*Name, nullptr) : nullptr;
}

void ANetBaseCharacter::UpdateBodyParts()
{
	ChangeBodyPart(EBodyPart::BP_FACE, 0, false);
	ChangeBodyPart(EBodyPart::BP_BEARD, 0, false);
	ChangeBodyPart(EBodyPart::BP_CHEST, 0, false);
	ChangeBodyPart(EBodyPart::BP_HAIR, 0, false);
	ChangeBodyPart(EBodyPart::BP_HANDS, 0, false); 
	ChangeBodyPart(EBodyPart::BP_LEGS, 0, false);
	ChangeBodyPart(EBodyPart::BP_EYEBROWS, 0, false);
}

// Called to bind functionality to input
void ANetBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

