
#include "Components/CombatComponent.h"

#include "Characters/TPSCharacter.h"
#include "Components/SphereComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Net/UnrealNetwork.h"
#include "Weapons/Weapon.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, EquippedWeapon);
}

void UCombatComponent::EquipWeapon(AWeapon* WeaponToEquip)
{
	if(TPSCharacter && WeaponToEquip)
	{
		//Get right hand socket for a new weapon
		const USkeletalMeshSocket* WeaponSocket = TPSCharacter->GetMesh()->GetSocketByName(FName("RightHandSocket"));
		if(WeaponSocket)
		{
			//Equip the weapon if attached successfully 
			if(WeaponSocket->AttachActor(WeaponToEquip, TPSCharacter->GetMesh()))
			{
				EquippedWeapon = WeaponToEquip;
				EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
				EquippedWeapon->SetOwner(TPSCharacter);

				TPSCharacter->Server_UpdateAnimInstanceClass(true);
			}
		}
	}
}
