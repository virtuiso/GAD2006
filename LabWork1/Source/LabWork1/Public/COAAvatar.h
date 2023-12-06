// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "COABaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "COAAvatar.generated.h"

/**
 *
 */
UCLASS()
class ACOAAvatar : public ACOABaseCharacter
{
	GENERATED_BODY()


public:

	ACOAAvatar(); // constructor for this class

	UPROPERTY(EditAnywhere, Category = "COA")
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, Category = "COA")
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "COA")
	float RunningSpeed;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	/**/
	UPROPERTY(EditAnywhere, Category = "COA")
	float MaxStamina;

	UPROPERTY(EditAnywhere, Category = "COA")
	float StaminaDrainRate;

	UPROPERTY(EditAnywhere, Category = "COA")
	float StaminaGainRate;
	/**/



private: // can add if you want to hide smth from other classes

	void MoveForward(float Amount);
	void MoveRight(float Amount);

	void RunPressed();
	void RunReleased();

	/**/
	void UpdateMovementParams();

	UPROPERTY(VisibleAnywhere, Category = "COA") // read-only variable
		float Stamina;

	UPROPERTY(VisibleAnywhere, Category = "COA")
	bool bHoldingRunKey;

	UPROPERTY(VisibleAnywhere, Category = "COA")
	bool bStaminaDrained;

	UPROPERTY(VisibleAnywhere, Category = "COA")
	bool bReleased;
	/**/


};
