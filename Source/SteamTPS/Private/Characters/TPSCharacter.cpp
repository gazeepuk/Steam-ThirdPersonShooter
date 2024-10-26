// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/TPSCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "Components/CombatComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "UI/DisplayTextWidget.h"
#include "Weapons/Weapon.h"

// Sets default values
ATPSCharacter::ATPSCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->TargetArmLength = 500.f;
	SpringArmComponent->bUsePawnControlRotation = true;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	OverheadNameWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OerheadNameWidget"));
	OverheadNameWidget->SetupAttachment(GetRootComponent());

	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	CombatComponent->SetIsReplicated(true);
}

// Called to bind functionality to input
void ATPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Binds Movement actions
		if(IsValid(MoveAction))
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
		}
		if(IsValid(LookAction))
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
		}
		if(IsValid(JumpAction))
		{
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ThisClass::StartJumpInput);
		}
		if(IsValid(CrouchAction))
		{
			EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ThisClass::CrouchInput);
			EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &ThisClass::UnCrouchInput);
		}
		if(IsValid(EquipAction))
		{
			EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Started, this, &ThisClass::EquipInput);
		}
	}
}

void ATPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
}

void ATPSCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(ThisClass, OverlappingWeapon, COND_OwnerOnly);
}

void ATPSCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if(CombatComponent)
	{
		CombatComponent->TPSCharacter = this;
	}
}

void ATPSCharacter::SetOverheadPlayerName()
{
	//Set Player's name to the display text widget
	UDisplayTextWidget* DisplayTextWidget = OverheadNameWidget->GetWidget() ? Cast<UDisplayTextWidget>(OverheadNameWidget->GetWidget()) : nullptr;
	if(DisplayTextWidget && GetPlayerState())
	{
		const FString PlayerName = GetPlayerState()->GetPlayerName();
		DisplayTextWidget->SetDisplayText(FText::FromString(PlayerName));
	}
}

#pragma region Input Callbacks

void ATPSCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ATPSCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ATPSCharacter::StartJumpInput(const FInputActionValue& Value)
{
	Jump();
}

void ATPSCharacter::CrouchInput(const FInputActionValue& Value)
{
	Crouch();
}

void ATPSCharacter::UnCrouchInput(const FInputActionValue& Value)
{
	UnCrouch();
}

void ATPSCharacter::EquipInput(const FInputActionValue& Value)
{
	Server_EquipWeapon();
}

#pragma endregion

void ATPSCharacter::OnRep_OverlappingWeapon(AWeapon* OldWeapon)
{
	//Show new weapon's PickUp widget
	if(OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickUpWidget(true);
	}
	//Hide old weapon's PickUp widget
	if(OldWeapon)
	{
		OldWeapon->ShowPickUpWidget(false);
	}
}

void ATPSCharacter::SetOverlappingWeapon(AWeapon* Weapon)
{
	//Hide previous overlapped weapon
	if(OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickUpWidget(false);
	}

	//Set new overlapped weapon
	OverlappingWeapon = Weapon;
	if(IsLocallyControlled())
	{
		//Show weapon's PickUp widget
		if(OverlappingWeapon)
		{
			OverlappingWeapon->ShowPickUpWidget(true);
		}
	}
}

void ATPSCharacter::Server_EquipWeapon_Implementation()
{
	if(CombatComponent)
	{
		CombatComponent->EquipWeapon(OverlappingWeapon);
	}
}

