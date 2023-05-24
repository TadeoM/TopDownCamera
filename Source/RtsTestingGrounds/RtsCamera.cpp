// Fill out your copyright notice in the Description page of Project Settings.


#include "RtsCamera.h"
#include "Math/Vector2D.h"
#include "DrawDebugHelpers.h"
#include <GameFramework/SpringArmComponent.h>

// Sets default values
ARtsCamera::ARtsCamera()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ARtsCamera::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			subsystem->AddMappingContext(cameraMappingContext, 0);
		}
	}
	
	springArmComponent = FindComponentByClass<USpringArmComponent>();
	SetActorLocation(FVector(0,0,15));
}

// Called to bind functionality to input
void ARtsCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARtsCamera::Move);
		EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Triggered, this, &ARtsCamera::Rotate);
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &ARtsCamera::Zoom);
		EnhancedInputComponent->BindAction(MiddleClickRotateAction, ETriggerEvent::Triggered, this, &ARtsCamera::RotateWithMousePosition);
	}
}
// Called every frame
void ARtsCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARtsCamera::Move(const FInputActionValue& value)
{
	const FVector2D currentValue = value.Get<FVector2D>();

 	if (!currentValue.IsZero())
	{
		const FRotator rotation = Controller->GetControlRotation();
		const FRotator yawRotation(0, rotation.Yaw, 0);

		// get forward vector
		const FVector forwardDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector rightDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(forwardDirection, currentValue.Y);
		AddMovementInput(rightDirection, currentValue.X);
	}
}

void ARtsCamera::Rotate(const FInputActionValue& value)
{
	const float rotateValue = value.Get<float>();

	if (GetController())
	{
		AController* controller = GetController();
		FRotator rotator(controller->GetControlRotation().Pitch, controller->GetControlRotation().Yaw + rotateValue, controller->GetControlRotation().Roll);

		controller->SetControlRotation(rotator);
	}
}

/// <summary>
///	Check if making a rotation by pitchRotation will go beyond the 
/// </summary>
/// <param name="pitchRotation"></param>
/// <returns></returns>
bool ARtsCamera::CanRotatePitch(double pitchToRotateBy)
{
	FRotator controllerRotation = GetController()->GetControlRotation();
	
	// rotates down
	if (pitchToRotateBy < 0)
	{
		if (controllerRotation.Pitch - pitchToRotateBy >= 50.0f)
		{
			return false;
		}
	}
	// rotates up
	else if (pitchToRotateBy > 0)
	{
		if (controllerRotation.Pitch - pitchToRotateBy <= 0.0f)
		{
			return false;
		}
	}
	
	return true;
}

void ARtsCamera::RotateWithMousePosition(const FInputActionValue& value)
{
	const FVector2D rotateValue = value.Get<FVector2D>();

	if (AController* controller = GetController())
	{
		FRotator rotation = GetController()->GetControlRotation();
		AddControllerYawInput(rotateValue.X);

		if (CanRotatePitch(rotateValue.Y))
		{
			double lastRotation = rotation.Pitch;
			AddControllerPitchInput(rotateValue.Y);
			double newRotation = rotation.Pitch;

			// this is a nice safeguard because sometimes rotating below 0 will set rotation to 360, and this messes up calculations
			if (newRotation > 51.0f)
			{
				FRotator rotator(0.0f, controller->GetControlRotation().Yaw, controller->GetControlRotation().Roll);
				controller->SetControlRotation(rotator);
			}
		}
	}
}

void ARtsCamera::Zoom(const FInputActionValue& value)
{
	const float zoomValue = value.Get<float>();

	if (zoomValue)
	{
		springArmComponent->TargetArmLength += zoomValue * zoomSpeed;
		springArmComponent->TargetArmLength = FMath::Clamp(springArmComponent->TargetArmLength, closestZoomValue, furthestZoomValue);
	}
}



