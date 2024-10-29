// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Components/CombatComponent.h"
#include "GameFramework/Character.h"
#include "Weapons/Weapon.h"
#include "TPSCharacter.generated.h"

class UTPSAnimInstance;
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
private:
	virtual void Tick(float DeltaSeconds) override;
	//~Animations
private:
	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	TSubclassOf<UTPSAnimInstance> UnequippedAnimInstanceClass;
	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	TSubclassOf<UTPSAnimInstance> ArmedAnimInstanceClass;
public:
	UFUNCTION(Server, Reliable)
	void Server_UpdateAnimInstanceClass(bool bArmed);
	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_UpdateAnimInstanceClass(TSubclassOf<UTPSAnimInstance> NewAnimInstanceClass);
	
	// ~Input
public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	//Mapping Context
	UPROPERTY(EditDefaultsOnly, Category = "Input|InputAction")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	//Input actions
	UPROPERTY(EditDefaultsOnly, Category = "Input|InputAction|Movement")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input|InputAction|Movement")
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input|InputAction|Movement")
	TObjectPtr<UInputAction> JumpAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input|InputAction|Movement")
	TObjectPtr<UInputAction> CrouchAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input|InputAction|Combat")
	TObjectPtr<UInputAction> EquipAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input|InputAction|Combat")
	TObjectPtr<UInputAction> AimAction;
	
	//Functions called for input
	virtual void Move(const FInputActionValue& Value);
	virtual void Look(const FInputActionValue& Value);
	virtual void StartJumpInput(const FInputActionValue& Value);
	virtual void CrouchInput(const FInputActionValue& Value);
	virtual void UnCrouchInput(const FInputActionValue& Value);
	virtual void EquipInput(const FInputActionValue& Value);
	virtual void StartAimInput(const FInputActionValue& Value);
	virtual void StopAimInput(const FInputActionValue& Value);

	// ~Weapon
private:
	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	TObjectPtr<AWeapon> OverlappingWeapon;

	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* OldWeapon);
public:
	FORCEINLINE void SetOverlappingWeapon(AWeapon* Weapon);
	bool IsWeaponEquipped() const;
	bool IsAiming() const;
	
	// ~Combat
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCombatComponent> CombatComponent;
	
	UFUNCTION(Server, Reliable)
	void Server_EquipWeapon();

	// ~AimOffset
	void CalculateAimOffset(float DeltaTime);

	float AO_Yaw;
	float AO_Pitch;
	FRotator StartingAimRotation;
public:
	FORCEINLINE float Get_AO_Yaw() const {return AO_Yaw;}
	FORCEINLINE float Get_AO_Pitch() const {return AO_Pitch;}
};
