// Fill out your copyright notice in the Description page of Project Settings.


#include "COABaseCharacter.h"

// Sets default values
// Constructor is executed once the object is created.
ACOABaseCharacter::ACOABaseCharacter() : Health(0.0f), MaxHealth(0.0f), HealingRate(10.0f),
WalkingSpeed(150.0f)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// MaxHealth = 100; // any character will start with MaxHealth = 100

	GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed;
}

void ACOABaseCharacter::setHealth(float NewHealth)
{
	Health = NewHealth;
}

float ACOABaseCharacter::getHealth()
{
	return Health;
}

// Called when the game starts or when spawned
void ACOABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->bOrientRotationToMovement = true; // makes it so that movement rotation is synchronized with the camera rotation

}

// construction script will execute anytime a change is made on the object (unlike Constructor)
void ACOABaseCharacter::OnConstruction(const FTransform& Transform)
{
	Health = MaxHealth; // when character spawns construction script will set health to maxhealth
	Update = false;

}

// Called every frame
void ACOABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Health = FMath::Min(MaxHealth, Health + HealingRate * DeltaTime); // every second (each game tick) the character will be healed at some rate

}
