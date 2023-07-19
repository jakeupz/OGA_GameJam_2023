// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/BulletHitInterface.h"
#include "Enemy.generated.h"

class UAnimMontage;
class UPawnSensingComponent;

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	EES_Holding UMETA(DisplayName = "Holding"),
	EES_Patrolling UMETA(DisplayName = "Patrolling"),
	EES_Chasing UMETA(DisplayName = "Chasing"),
	EES_Attacking UMETA(DisplayName = "Attacking")
};

//, public IBulletHitInterface
UCLASS()
class OGA_GAMEJAM_2023_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();
	void CheckPatrolTarget();
	void CheckCombatTarget();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* Particles to spawn when hit by bullets 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		class UParticleSystem* ImpactParticles;*/

	/* Sound to play when hit by bulelts
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class USoundCue* ImpactSound;*/

	/** Current health of the enemy */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
		float Health;

	/** Maximum health of the enemy */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		float MaxHealth;

	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Fire(const FVector& HitTarget);

//	virtual void BulletHit_Implementation(FHitResult HitResult) override;

//	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:

/*
	Components
*/

	UPROPERTY(VisibleAnywhere)
		UPawnSensingComponent* PawnSensing;

/*
	Patrol mechanics
*/

	UPROPERTY()
		AActor* CombatTarget;

	UPROPERTY(EditAnywhere)
		double CombatRadius = 500.f;

	UPROPERTY(EditAnywhere)
		double AttackRadius = 150.f;

	/**
	* Navigation
	*/

	UPROPERTY()
		class AAIController* EnemyController;

	// Current patrol target
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
		AActor* PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
		TArray<AActor*> PatrolTargets;

	UPROPERTY(EditAnywhere)
		double PatrolRadius = 200.f;

	FTimerHandle PatrolTimer;
	void PatrolTimerFinished();

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
		float WaitMin = 5.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
		float WaitMax = 10.f;

	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

protected:
	void Die();
	bool InTargetRange(AActor* Target, double Radius);
	void MoveToTarget(AActor* Target);
	AActor* ChoosePatrolTarget();

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);
};
