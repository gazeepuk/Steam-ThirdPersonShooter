// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Weapon.h"

#include "Characters/TPSCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(GetRootComponent());
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	WeaponMesh->SetCollisionResponseToAllChannels(ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	
	GrabSphere = CreateDefaultSubobject<USphereComponent>(TEXT("GrabSphere"));
	GrabSphere->SetupAttachment(WeaponMesh);
	GrabSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GrabSphere->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	
	PickUpWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickUpWidget"));
	PickUpWidgetComponent->SetupAttachment(WeaponMesh);
	PickUpWidgetComponent->SetVisibility(false);
}

void AWeapon::ShowPickUpWidget(const bool bShowWidget) const
{
	if(PickUpWidgetComponent)
	{
		PickUpWidgetComponent->SetVisibility(bShowWidget);
	}
}

void AWeapon::SetWeaponState(const EWeaponState NewWeaponState)
{
	WeaponState = NewWeaponState;
	switch (WeaponState)
	{
	case EWeaponState::EWS_Initial:
		break;
	case EWeaponState::EWS_Equipped:
		ShowPickUpWidget(false);
		GrabSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	case EWeaponState::EWS_Dropped:
		break;
	case EWeaponState::EWS_MAX:
		break;
	}
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority())
	{
		GrabSphere->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
		GrabSphere->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
		GrabSphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnGrabSphereBeginOverlap);
		GrabSphere->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnGrabSphereEndOverlap);
	}
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, WeaponState);
}

void AWeapon::OnGrabSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATPSCharacter* TPSCharacter = OtherActor ? Cast<ATPSCharacter>(OtherActor) : nullptr;
	if(TPSCharacter && PickUpWidgetComponent)
	{
		TPSCharacter->SetOverlappingWeapon(this);
	}
}

void AWeapon::OnGrabSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ATPSCharacter* TPSCharacter = OtherActor ? Cast<ATPSCharacter>(OtherActor) : nullptr;
	if(TPSCharacter && PickUpWidgetComponent)
	{
		TPSCharacter->SetOverlappingWeapon(nullptr);
	}
}

void AWeapon::OnRep_WeaponState()
{
	switch(WeaponState) {
	case EWeaponState::EWS_Initial:
		break;
	case EWeaponState::EWS_Equipped:
		ShowPickUpWidget(false);
		break;
	case EWeaponState::EWS_Dropped:
		break;
	case EWeaponState::EWS_MAX:
		break;
	}

}
