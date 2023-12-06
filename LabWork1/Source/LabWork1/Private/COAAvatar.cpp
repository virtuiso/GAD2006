// Fill out your copyright notice in the Description page of Project Settings.


#include "COAAvatar.h"

// constructor implementation
ACOAAvatar::ACOAAvatar() : RunningSpeed(600.0f), MaxStamina(100.0f), StaminaDrainRate(10.0f),
StaminaGainRate(15.0f), Stamina(100.0f), bHoldingRunKey(false), bStaminaDrained(false), bReleased(false) // can initialize variables in a more compact way like this
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	// we just want the SpringArm to be used for rotation with mouse, and we don't want anything else to be affected
	SpringArm->bUsePawnControlRotation = true;
	Camera->bUsePawnControlRotation = false; // by default it may be true so we make sure we disable it
	bUseControllerRotationYaw = false;
}

// Called to bind functionality to input
void ACOAAvatar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn", this, &ACharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Lookup", this, &ACharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACOAAvatar::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACOAAvatar::MoveRight); // we use the functions that we created, instead of the movement
	// functions readily available in ACharacter class.
	// & means the reference to this function

	PlayerInputComponent->BindAction("Run", EInputEvent::IE_Pressed, this, &ACOAAvatar::RunPressed);
	PlayerInputComponent->BindAction("Run", EInputEvent::IE_Released, this, &ACOAAvatar::RunReleased);


}

void ACOAAvatar::MoveForward(float Amount)
{
	FRotator ControlRotation = GetControlRotation();
	FVector ForwardDirection = FRotationMatrix(FRotator(0.0f, ControlRotation.Yaw, 0.0f)).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, Amount);

}

void ACOAAvatar::MoveRight(float Amount)
{
	FRotator ControlRotation = GetControlRotation();
	FVector RightDirection = FRotationMatrix(FRotator(0.0f, ControlRotation.Yaw, 0.0f)).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, Amount);

}

void ACOAAvatar::RunPressed()
{

	bHoldingRunKey = true;
	UpdateMovementParams();

}

void ACOAAvatar::RunReleased()
{

	bHoldingRunKey = false;
	UpdateMovementParams();

}

void ACOAAvatar::BeginPlay()
{
	Super::BeginPlay();
}

void ACOAAvatar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Stamina <= 0.0f) {
		bStaminaDrained = true;
		UpdateMovementParams();
	}
	else if (Stamina >= MaxStamina) {
		bStaminaDrained = false;
		UpdateMovementParams();
	}


	if (!(GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Walking) && (bStaminaDrained == false)) {

		if (!(GetCharacterMovement()->Velocity.IsZero()) && bHoldingRunKey) { // only decrease if actually running (not when just pressing shift while standing)
			Stamina = FMath::Max(0.0f, Stamina - StaminaDrainRate * DeltaTime);
		}
		else {
			Stamina = FMath::Min(MaxStamina, Stamina + StaminaGainRate * DeltaTime);
		}

	}
	else {
		if (!(GetCharacterMovement()->Velocity.IsZero()) && bHoldingRunKey && !bStaminaDrained) { // only decrease if actually running (not when just pressing shift while standing)
			Stamina = FMath::Max(0.0f, Stamina - StaminaDrainRate * DeltaTime);
		}
		else {
			Stamina = FMath::Min(MaxStamina, Stamina + StaminaGainRate * DeltaTime);
		}

	}
	GEngine->AddOnScreenDebugMessage(0, DeltaTime, FColor::White, FString::Printf(TEXT("Stamina: %f"), Stamina));

}

void ACOAAvatar::UpdateMovementParams()
{
	GetCharacterMovement()->MaxWalkSpeed = bHoldingRunKey && !bStaminaDrained ? RunningSpeed : WalkingSpeed;

}