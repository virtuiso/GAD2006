// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "COABaseCharacter.generated.h"

UCLASS()
class ACOABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACOABaseCharacter();

	//UPROPERTY(EditAnywhere, Category = "COA") // can be edited anywhere but cannot use "get & set" in event graph
	//float Health;

	UPROPERTY(VisibleAnywhere, Category = "COA") // read-only variable
		float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "COA")
	float MaxHealth;

	UFUNCTION(BlueprintCallable, Category = "COA")
	void setHealth(float NewHealth);

	UFUNCTION(BlueprintPure, Category = "COA")
	float getHealth();
	// BlueprintCallable used to be able to call the function inside event graph, as usual 
	// There is also BlueprintPure, which can be used only for funtions returning a value. Pure functions don't have execution pins.

	UPROPERTY(EditAnywhere, Category = "COA")
	bool Update;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "COA")
	float HealingRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "COA")
	float WalkingSpeed;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// override the construction script
	virtual void OnConstruction(const FTransform& Transform) override; // OnConstruction will be underlined with green.
	// alt+enter click on it and create definition in .cpp


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;



};