// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/AnimInstances/TPSAnimInstance.h"

#include "Characters/TPSCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"

void UTPSAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	TPSCharacter = TryGetPawnOwner() ? Cast<ATPSCharacter>(TryGetPawnOwner()) : nullptr;
	CharacterMovementComponent = TPSCharacter ? TPSCharacter->GetCharacterMovement() : nullptr;
}

void UTPSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	TPSCharacter = TryGetPawnOwner() ? Cast<ATPSCharacter>(TryGetPawnOwner()) : nullptr;


	Velocity = TPSCharacter ? TPSCharacter->GetVelocity() : FVector::ZeroVector;
	GroundSpeed = Velocity.Size2D();
	bFalling = CharacterMovementComponent && CharacterMovementComponent->IsFalling();
	bCrouching = CharacterMovementComponent && CharacterMovementComponent->IsCrouching();
	bAccelerating = CharacterMovementComponent && CharacterMovementComponent->GetCurrentAcceleration().Size() > 0;
	bShouldMove = bAccelerating && GroundSpeed > 3.f;
	bWeaponEquipped = TPSCharacter && TPSCharacter->IsWeaponEquipped();
}
