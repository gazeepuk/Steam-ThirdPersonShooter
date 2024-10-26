// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class UWidgetComponent;
class USphereComponent;


UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Initial UMETA(DisplayName = "Initial State"),
	EWS_Equipped UMETA(DisplayName = "Equipped"),
	EWS_Dropped UMETA(DisplayName = "Dropped"),

	EWS_MAX UMETA(DisplayName = "DefaultMax")
};

UCLASS()
class STEAMTPS_API AWeapon : public AActor
{
	GENERATED_BODY()
public:
	AWeapon();

	void ShowPickUpWidget(bool bShowWidget) const;

	void SetWeaponState(const EWeaponState NewWeaponState);
	FORCEINLINE USphereComponent* GetGrabSphere() const { return GrabSphere; }
protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION()
	virtual void OnGrabSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	virtual void OnGrabSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> WeaponMesh;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USphereComponent> GrabSphere;
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_WeaponState)
	EWeaponState WeaponState;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UWidgetComponent> PickUpWidgetComponent;

	UFUNCTION()
	void OnRep_WeaponState();
};
