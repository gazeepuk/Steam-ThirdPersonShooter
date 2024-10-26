// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Weapons/Weapon.h"
#include "TPSCharacter.generated.h"

class UCombatComponent;
class AWeapon;
class UWidgetComponent;
class UInputAction;
class UInputMappingContext;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class STEAMTPS_API ATPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATPSCharacter();

	//Set Player's name above the character
	UFUNCTION(BlueprintCallable)
	void SetOverheadPlayerName();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UCameraComponent> CameraComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> OverheadNameWidget;

	// ~Input
public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	//Mapping Context
	UPROPERTY(EditDefaultsOnly, Category = "Input|InputAction")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	//Input actions
	UPROPERTY(EditDefaultsOnly, Category = "Input|InputAction")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input|InputAction")
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input|InputAction")
	TObjectPtr<UInputAction> JumpAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input|InputAction")
	TObjectPtr<UInputAction> CrouchAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input|InputAction")
	TObjectPtr<UInputAction> EquipAction;
	
	//Functions called for input
	virtual void Move(const FInputActionValue& Value);
	virtual void Look(const FInputActionValue& Value);
	virtual void StartJumpInput(const FInputActionValue& Value);
	virtual void CrouchInput(const FInputActionValue& Value);
	virtual void UnCrouchInput(const FInputActionValue& Value);
	virtual void EquipInput(const FInputActionValue& Value);

	// ~Weapon
private:
	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	TObjectPtr<AWeapon> OverlappingWeapon;

	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* OldWeapon);
public:
	FORCEINLINE void SetOverlappingWeapon(AWeapon* Weapon);

	// ~Combat
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCombatComponent> CombatComponent;

	UFUNCTION(Server, Reliable)
	void Server_EquipWeapon();
};
