// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TPSAnimInstance.generated.h"

class UCharacterMovementComponent;
class ATPSCharacter;
/**
 * 
 */
UCLASS()
class STEAMTPS_API UTPSAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:

	//References
	UPROPERTY(BlueprintReadOnly, Category = "References")
	TObjectPtr<ATPSCharacter> TPSCharacter;
	UPROPERTY(BlueprintReadOnly, Category = "References")
	TObjectPtr<UCharacterMovementComponent> CharacterMovementComponent;

	//Movement Data
	UPROPERTY(BlueprintReadOnly, Category = "Movement Data")
	float GroundSpeed = 0.f;
	UPROPERTY(BlueprintReadOnly, Category = "Movement Data")
	FVector Velocity;
	UPROPERTY(BlueprintReadOnly, Category = "Movement Data")
	bool bFalling = false;
	UPROPERTY(BlueprintReadOnly, Category = "Movement Data")
	bool bAccelerating = false;
	UPROPERTY(BlueprintReadOnly, Category = "Movement Data")
	bool bShouldMove = false;
	UPROPERTY(BlueprintReadOnly, Category = "Movement Data")
	bool bCrouching = false;
	UPROPERTY(BlueprintReadOnly, Category = "Movement Data")
	float YawOffset = 0.f;
	UPROPERTY(BlueprintReadOnly, Category = "Movement Data")
	float Lean = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Combat Data")
	bool bWeaponEquipped = false;
	UPROPERTY(BlueprintReadOnly, Category = "Combat Data")
	bool bAiming = false;

	UPROPERTY(BlueprintReadOnly, Category = "AimOffset Data")
	float AO_Yaw;
	UPROPERTY(BlueprintReadOnly, Category = "AimOffset Data")
	float AO_Pitch;	

	FRotator CharacterRotationLastFrame; 
	FRotator CharacterRotation; 
	FRotator DeltaRotation;
};
