// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputMappingContext.h"
#include "InputActionValue.h"
#include <EnhancedInputSubsystems.h>
#include <EnhancedInputComponent.h>
#include "RtsCamera.generated.h"

class USpringArmComponent;

UCLASS()
class ARtsCamera : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ARtsCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#pragma region Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* cameraMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* RotateAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* ZoomAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		float zoomSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		float closestZoomValue;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		float furthestZoomValue;
	
	void Move(const FInputActionValue& value);
	void Rotate(const FInputActionValue& value);
	void Zoom(const FInputActionValue& value);
#pragma endregion

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		USpringArmComponent* springArmComponent;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
