// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/AnimInstances/TPSAnimInstance.h"

#include "Characters/TPSCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UTPSAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	TPSCharacter = TryGetPawnOwner() ? Cast<ATPSCharacter>(TryGetPawnOwner()) : nullptr;
	CharacterMovementComponent = TPSCharacter ? TPSCharacter->GetCharacterMovement() : nullptr;
}

void UTPSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(!TPSCharacter)
	{
		TPSCharacter = TryGetPawnOwner() ? Cast<ATPSCharacter>(TryGetPawnOwner()) : nullptr;
		if(!CharacterMovementComponent)
		{
			CharacterMovementComponent = TPSCharacter ? TPSCharacter->GetCharacterMovement() : nullptr;
		}
	}

	if(!TPSCharacter || !CharacterMovementComponent)
	{
		Velocity = FVector::ZeroVector;
		GroundSpeed = 0.f;
		bFalling = false;
		bCrouching = false;
		bAccelerating = false;
		bShouldMove = false;
		bWeaponEquipped = false;
		bAiming = false;
		YawOffset = 0.f;
		Lean = 0.f;
		return;
	}
	
	Velocity = TPSCharacter->GetVelocity();
	GroundSpeed = Velocity.Size2D();
	bFalling = CharacterMovementComponent->IsFalling();
	bCrouching = CharacterMovementComponent->IsCrouching();
	bAccelerating = CharacterMovementComponent->GetCurrentAcceleration().Size() > 0;
	bShouldMove = bAccelerating && GroundSpeed > 3.f;
	bWeaponEquipped = TPSCharacter->IsWeaponEquipped();
	bAiming = TPSCharacter->IsAiming();

	//Calculation YawOffset for strafing
	FRotator AimRotation = TPSCharacter->GetBaseAimRotation();
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(Velocity);
	FRotator DeltaOffsetRotation = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);
	DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaOffsetRotation, DeltaSeconds, 6.f);
	YawOffset = DeltaRotation.Yaw;
	
	//Calculating Lean
	CharacterRotationLastFrame = CharacterRotation;
	CharacterRotation = TPSCharacter->GetActorRotation();
	const FRotator DeltaCharacterRotation = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);
	const float Target = DeltaCharacterRotation.Yaw / DeltaSeconds;
	const float Interp = FMath::FInterpTo(Lean, Target, DeltaSeconds, 6.f);
	Lean = FMath::Clamp(Interp, -90.f, 90.f);
}
