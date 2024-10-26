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

	void EquipWeapon(AWeapon* WeaponToEquip);
private:
	TObjectPtr<AWeapon> EquippedWeapon;	
	TObjectPtr<ATPSCharacter> TPSCharacter;
};
