// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Components/EnemyCombatComponent.h"
#include "Enemy/Enemy.h"
#include "Character/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

UEnemyCombatComponent::UEnemyCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UEnemyCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UEnemyCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UEnemyCombatComponent::Fire()
{
	if (bCanFire)
	{
		bCanFire = false;
		FHitResult HitResult;
		TraceUnderCrosshairs(HitResult);
	}
}

void UEnemyCombatComponent::TraceUnderCrosshairs(FHitResult& TraceHitResult)
{
	FVector Start = Enemy->GetActorLocation();

	Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	FVector End = Start + Player->GetActorLocation() * TRACE_LENGTH;

	GetWorld()->LineTraceSingleByChannel(
		TraceHitResult,
		Start,
		End,
		ECollisionChannel::ECC_Visibility
	);
}

void UEnemyCombatComponent::StartFireTimer()
{
	if (Enemy == nullptr) return;
	Enemy->GetWorldTimerManager().SetTimer(
		FireTimer,
		this,
		&UEnemyCombatComponent::FireTimerFinished,
		FireDelay
	);
}

void UEnemyCombatComponent::FireTimerFinished()
{
	if (Enemy == nullptr) return;
	bCanFire = true;
	Fire();
}
