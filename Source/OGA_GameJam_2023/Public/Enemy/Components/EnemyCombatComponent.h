// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyCombatComponent.generated.h"

#define TRACE_LENGTH 80000.f


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OGA_GAMEJAM_2023_API UEnemyCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEnemyCombatComponent();

	class AEnemy* Enemy;

protected:
	virtual void BeginPlay() override;

	void Fire();

	void TraceUnderCrosshairs(FHitResult& TraceHitResult);


public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	class ACharacter* Player;

	/**
	* Automatic fire
	*/

	FTimerHandle FireTimer;

	UPROPERTY(EditAnywhere, Category = Combat)
	bool bCanFire = true;

	void StartFireTimer();
	void FireTimerFinished();

	UPROPERTY(EditAnywhere, Category = Combat)
	float FireDelay = .5f;
};
