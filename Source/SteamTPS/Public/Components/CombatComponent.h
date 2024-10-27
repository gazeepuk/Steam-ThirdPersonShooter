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
private:
	UPROPERTY(Replicated)
	TObjectPtr<AWeapon> EquippedWeapon;	
	TObjectPtr<ATPSCharacter> TPSCharacter;
};
