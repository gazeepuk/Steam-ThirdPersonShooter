// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Weapon.h"

#include "Characters/TPSCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"

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
