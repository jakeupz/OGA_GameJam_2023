// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/PlayerAnimInstance.h"
#include "Character/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon/Weapon.h"

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (PlayerCharacter == nullptr)
	{
		PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
	}
	if (PlayerCharacter == nullptr) return;

	FVector Velocity = PlayerCharacter->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();

	bIsInAir = PlayerCharacter->GetCharacterMovement()->IsFalling();
	bIsAccelerating = PlayerCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;
	bWeaponEquipped = PlayerCharacter->IsWeaponEquipped();
	EquippedWeapon = PlayerCharacter->GetEquippedWeapon();
	bIsCrouched = PlayerCharacter->bIsCrouched;
	bAiming = PlayerCharacter->IsAiming();
	TurningInPlace = PlayerCharacter->GetTurningInPlace();

	// Offset Yaw for Strafing
	FRotator AimRotation = PlayerCharacter->GetBaseAimRotation();
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(PlayerCharacter->GetVelocity());
	FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);
	YawOffset = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw;

	CharacterRotationLastFrame = CharacterRotation;
	CharacterRotation = PlayerCharacter->GetActorRotation();
	const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);
	const float Target = Delta.Yaw / DeltaTime;
	const float Interp = FMath::FInterpTo(Lean, Target, DeltaTime, 6.f);
	Lean = FMath::Clamp(Interp, -90.f, 90.f);

	AO_Yaw = PlayerCharacter->GetAO_Yaw();
	AO_Pitch = PlayerCharacter->GetAO_Pitch();

	if (bWeaponEquipped && EquippedWeapon && EquippedWeapon->GetWeaponMesh() && PlayerCharacter->GetMesh())
	{
		LeftHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("LeftHandSocket"), ERelativeTransformSpace::RTS_World);
		FVector OutPosition;
		FRotator OutRotation;
		PlayerCharacter->GetMesh()->TransformToBoneSpace(FName("hand_r"), LeftHandTransform.GetLocation(), FRotator::ZeroRotator, OutPosition, OutRotation);
		LeftHandTransform.SetLocation(OutPosition);
		LeftHandTransform.SetRotation(FQuat(OutRotation));

		// Checks if the gun matches where the character is pointing
				FTransform MuzzleTipTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("MuzzleFlash"), ERelativeTransformSpace::RTS_World);
				FVector MuzzleX(FRotationMatrix(MuzzleTipTransform.GetRotation().Rotator()).GetUnitAxis(EAxis::X));
				DrawDebugLine(GetWorld(), MuzzleTipTransform.GetLocation(), MuzzleTipTransform.GetLocation() + MuzzleX * 1000.f, FColor::Red);
				DrawDebugLine(GetWorld(), MuzzleTipTransform.GetLocation(), PlayerCharacter->GetHitTarget(), FColor::Orange);
			
		FTransform RightHandTransform = PlayerCharacter->GetMesh()->GetSocketTransform(FName("hand_r"), ERelativeTransformSpace::RTS_World);

		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(RightHandTransform.GetLocation(), RightHandTransform.GetLocation() + (RightHandTransform.GetLocation() - PlayerCharacter->GetHitTarget()));

		LookAtRotation.Roll += PlayerCharacter->RightHandRotationRoll;
		LookAtRotation.Yaw += PlayerCharacter->RightHandRotationYaw;
		LookAtRotation.Pitch += PlayerCharacter->RightHandRotationPitch;

		RightHandRotation = FMath::RInterpTo(RightHandRotation, LookAtRotation, DeltaTime, 100.f);

	}
}