// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


class AWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STEAMTPS_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()
	friend class ATPSCharacter;

public:
	UCombatComponent();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	void EquipWeapon(AWeapon* WeaponToEquip);

protected:
	void SetIsAiming(bool bNewAiming);
	UFUNCTION(Server, Reliable)
	void Server_SetAiming(bool bNewAiming);

	UFUNCTION()
	void OnRep_EquippedWeapon();
private:
	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
	TObjectPtr<AWeapon> EquippedWeapon;	
	TObjectPtr<ATPSCharacter> TPSCharacter;

	UPROPERTY(Replicated)
	bool bAiming = false;
};
