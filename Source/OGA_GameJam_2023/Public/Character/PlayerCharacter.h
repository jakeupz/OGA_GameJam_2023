// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "OGA_GameJam_2023/PlayerCharacterTypes/TurningInPlace.h"
#include "PlayerCharacter.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class OGA_GAMEJAM_2023_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;

	void PlayFireMontage(bool bAiming);

protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, Category = Input)
		TObjectPtr<UInputMappingContext> PlayerContext;

	UPROPERTY(EditAnywhere, Category = Input)
		TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = Input)
		TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category = Input)
		TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category = Input)
		TObjectPtr<UInputAction> EquipAction;

	UPROPERTY(EditAnywhere, Category = Input)
		TObjectPtr<UInputAction> CrouchAction;

	UPROPERTY(EditAnywhere, Category = Input)
		TObjectPtr<UInputAction> AimAction;

	UPROPERTY(EditAnywhere, Category = Input)
		TObjectPtr<UInputAction> FireAction;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void EquipButtonPressed();
	void CrouchButtonPressed();
	void AimButtonPressed();
	void AimButtonReleased();
	void AimOffset(float DeltaTime);
	virtual void Jump() override;

	void FireButtonPressed();
	void FireButtonReleased();
	void PlayHitReactMontage();

	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser);
	void UpdateHUDHealth();

private:
	UPROPERTY(VisibleAnywhere, Category = Camera)
		class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere)
		class AWeapon* OverlappingWeapon;

	UPROPERTY(VisibleAnywhere)
		class UCombatComponent* Combat;

	float AO_Yaw;
	float InterpAO_Yaw;
	float AO_Pitch;
	FRotator StartingAimRotation;

	ETurningInPlace TurningInPlace;
	void TurnInPlace(float DeltaTime);

	UPROPERTY(EditAnywhere, Category = Combat)
	class UAnimMontage* FireWeaponMontage;

	// To do
	UPROPERTY(EditAnywhere, Category = Combat)
	class UAnimMontage* HitReactMontage;

	/**
	* Player health
	*/

	UPROPERTY(EditAnywhere, Category = "Player Stats")
		float MaxHealth = 100.f;

	UPROPERTY(VisibleAnywhere, Category = "Player Stats")
		float Health = 100.f;

	class APlayerCharacterController* PlayerCharacterController;

public:
	void SetOverlappingWeapon(AWeapon* Weapon);
	bool IsWeaponEquipped();
	bool IsAiming();
	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; }
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; }
	AWeapon* GetEquippedWeapon();
	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return TurningInPlace; }
};
