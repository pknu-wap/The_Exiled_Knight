// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "BehaviorTree/BehaviorTree.h"
#include"EnemyWeapon/EKEnemyWeaponBase.h"
#include "EK_EnemyBase.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FEnemyHpUpdated, int, int)
DECLARE_MULTICAST_DELEGATE(FEnemyDied)

UCLASS()
class THEEXILEDKNIGHT_API AEK_EnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEK_EnemyBase(); 


	FVector GetInitializeLocation(); 


	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInvestigator, AActor* DamageCauser)override;
	
	void BeginPlay()override;

	void InitStat();

	void PlayHurtReactionAnimation(const FVector& DamageDirection);   
	
	void OnHurtAnimationEnded(UAnimMontage* Montage, bool bInterrupted);

	void PlayDieReactionAnimation(); 

	void OnDeathAnimationEnded(UAnimMontage* Montage, bool bInterrupted);

	void PlayStunReactionAnimation();

	void OnStunAnimationEnded(UAnimMontage* Montage, bool bInterrupted);

	void HandleStrongAttack(float Damage);

	void HandleNormalAttack(float Damage);

	UFUNCTION(BlueprintCallable)
	AActor *GetAttackTarget();
	
	UFUNCTION(BlueprintCallable) 
	void SetAttackTarget(AActor* Actor);

	UFUNCTION(BlueprintCallable)
	void ReturnToInitializeLocation();

	UFUNCTION(BlueprintCallable)
	void StartBossBattle();
	
	UFUNCTION(BlueprintCallable)
	void ClearTarget();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon") 
	AActor* Weapon;
protected:

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "AnimMontage", meta = (AllowPrivateAccess = "true"));
	UAnimMontage* AttackAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimMontage", meta = (AllowPrivateAccess = "true"));
	UAnimMontage* hurtFAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimMontage", meta = (AllowPrivateAccess = "true"));
	UAnimMontage* hurtLAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimMontage", meta = (AllowPrivateAccess = "true"));
	UAnimMontage* hurtRAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimMontage", meta = (AllowPrivateAccess = "true"));
	UAnimMontage* hurtBAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimMontage", meta = (AllowPrivateAccess = "true"));
	UAnimMontage* DeathAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimMontage", meta = (AllowPrivateAccess = "true"));
	UAnimMontage* StunMontage;
public:	
	UPROPERTY(VisibleAnywhere, Category = Stat)
	class UEK_EnemyStatusComponent* EnemyStat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BehaviorTree");
	UBehaviorTree* BehaviorTree;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIPerception", meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> AttachedActors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIPerception", meta = (AllowPrivateAccess = "true"))
	AActor* AttackTarget;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	bool bIsStunned = false;


	virtual TObjectPtr <UEK_EnemyStatusComponent> GetStatusComponent(); 

	UAnimMontage* BeforeHurtMontage = nullptr;
	
	// Related to Sanctuary (Map)
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int SantuaryID = 0;

public:
	FTimerHandle RemoveTimeslowHandle;
	float TimeslowDuration = 0.1f;
	float TimeDelayValue = 0.5f;
	float DefaultTimeDelayValue = 1.f;
	void RemoveTimeslow();
	void RemoveTimeslowTimer();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AcceptanceRadius = 10.0f; 

public:
	FEnemyHpUpdated Delegate_HpUpdated;
	FEnemyDied Delegate_Died;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector InitialLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRotator InitialRotation;

public:
	UFUNCTION(BlueprintImplementableEvent)
	AActor* GetPatrolRoute();

	UFUNCTION(BlueprintImplementableEvent)
	void SetPatrolRoute(AActor* InPatrolRoute);

	void RestoreState();
};
