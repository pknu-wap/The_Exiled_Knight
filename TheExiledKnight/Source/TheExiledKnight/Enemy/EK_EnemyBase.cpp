// Fill out your copyright notice in the Description page of Project Settings.


#include "EK_EnemyBase.h"
#include "EK_EnemyStatusComponent.h"
#include"Player/Weapon/DamageType/EKPlayerDamageType.h"
#include"Enemy/EnemyAI/EKEnemyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/DamageEvents.h"
#include"Animation/AnimInstance.h"
#include"Player/Weapon/DamageType/EKPlayerDamageType.h"
#include"Player/EKPlayer/EKPlayer.h"
#include"Enemy/EKEnemyGamePlayTags.h"
#include"AIController.h"
#include "EKGameplayTags.h"
#include "UI/UISubsystem.h"
#include "Blueprint/UserWidget.h"
#include "UI/BossBattle/Widget_BossBattle.h"


// Sets default values
AEK_EnemyBase::AEK_EnemyBase()
{
	EnemyStat = CreateDefaultSubobject<UEK_EnemyStatusComponent>(TEXT("EnemyStat"));
}
void AEK_EnemyBase::BeginPlay()
{
	Super::BeginPlay();
	InitialLocation = GetActorLocation();
	InitialRotation = GetActorRotation();
}

#pragma region DamageSystem

float AEK_EnemyBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInvestigator, AActor* DamageCauser)
{
	const float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInvestigator, DamageCauser);

	TSubclassOf<UDamageType> DamageTypeClass = DamageEvent.DamageTypeClass;

	if (DamageCauser)
	{
		SetAttackTarget(DamageCauser);
	}
	AEKEnemyAIController* EnemyController = Cast<AEKEnemyAIController>(this);
	//if (EnemyController&&DamageCauser)
	//{
	//	UAISense_Damage::ReportDamageEvent(
	//		GetWorld(),
	//		this,               // ?��?지�?받�? Actor
	//		DamageCauser,       // ?��?지�?준 Actor
	//		DamageAmount,       // ?��?지 ??
	//		GetActorLocation(), // ?��?지 ?�치
	//		FVector::ZeroVector // ?��?지 방향
	//	);

	//}
	if (EnemyStat->GetCurrentHealth() > 0 && !EnemyStat->GetIsDead())
	{
		if (Damage > 0)
		{
			
			if (DamageTypeClass->IsChildOf(UEKPlayerStrongDamageType::StaticClass()))
			{
				HandleStrongAttack(Damage);
			}
			else //Normal and not 
			{
				HandleNormalAttack(Damage);
			}
			
			if (EnemyStat->GetCurrentHealth() <= 0)
			{
				EnemyStat->SetIsDead(true);
				Delegate_HpUpdated.Broadcast(EnemyStat->GetMaxHealth(), EnemyStat->GetCurrentHealth());
				Delegate_Died.Broadcast();
				PlayDieReactionAnimation();
				EnemyStat->OnHPIsZero.Broadcast();
				EnemyStat->OnHPIsZeroOneAstral.Broadcast(EnemyStat->GetAstral());
				//EnemyStat->OnHPIsZeroOneDropItemSource.Broadcast();
				return 0.0f;
			}

		
			FVector DamageDirection = (DamageCauser->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			PlayHurtReactionAnimation(DamageDirection);

			Delegate_HpUpdated.Broadcast(EnemyStat->GetMaxHealth(), EnemyStat->GetCurrentHealth());
		}
	}

	return Damage;
}

#pragma endregion
#pragma region HanadleDamageType
void AEK_EnemyBase::HandleStrongAttack(float Damage)
{
	EnemyStat->DamageCurrentHealth(Damage*1.2f);

	if (EnemyStat->GetCurrentPoise() > 0)
	{
		EnemyStat->DamageCurrentPoise(Damage*0.2f);
		
	}
}
void AEK_EnemyBase::HandleNormalAttack(float Damage)
{
	EnemyStat->DamageCurrentHealth(Damage); 

	if (EnemyStat->GetCurrentPoise() > 0)
	{
		EnemyStat->DamageCurrentPoise(Damage*0.1f);
	}
}

#pragma endregion

#pragma region  PlayHurtMontage

void AEK_EnemyBase::PlayHurtReactionAnimation(const FVector& DamageDirection)
{
	FVector Forward = GetActorForwardVector(); //forwar vector
	FVector Right = GetActorRightVector(); //right vector
	
	UAnimMontage* HurtMontage = nullptr;

	float  ForwardDot = FVector::DotProduct(Forward, DamageDirection);
	float RightDot = FVector::DotProduct(Right, DamageDirection);
		
	if (FMath::Abs(RightDot) > FMath::Abs(ForwardDot))
	{
		if (RightDot > 0)HurtMontage = hurtRAnimMontage;
		else HurtMontage = hurtLAnimMontage;
	}
	else
	{
		if (ForwardDot > 0)HurtMontage = hurtFAnimMontage;
		else HurtMontage = hurtBAnimMontage;
	}

	 
	if (HurtMontage) 
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance&&BeforeHurtMontage==nullptr)
		{
			if (AnimInstance->IsAnyMontagePlaying())AnimInstance->StopAllMontages(0.1f);  
			EnemyStat->OnDamageTaken.Broadcast();  
			FOnMontageEnded MontageEndedDelegate;
			MontageEndedDelegate.BindUObject(this, &AEK_EnemyBase::OnHurtAnimationEnded);
			AnimInstance->Montage_Play(HurtMontage); 
			BeforeHurtMontage = HurtMontage;
			AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, HurtMontage);
		
		}
	}
	else //don't have hurt montage 
	{
		EnemyStat->OnDamageTaken.Broadcast();
	}
			
}

void AEK_EnemyBase::OnHurtAnimationEnded(UAnimMontage* Montage, bool bInterrupted)
{
		
		UE_LOG(LogTemp, Warning, TEXT("OnHurtAnimationEnded called"));
		EnemyStat->OnHurtAnimationEnd.Broadcast();
		BeforeHurtMontage = nullptr;
		

	if (EnemyStat->GetCurrentPoise() <= 0 && !bIsStunned) //stun animation montage 
	{
		bIsStunned = true;
		PlayStunReactionAnimation();

	}
}

#pragma endregion
#pragma region PlayDieMontage

void AEK_EnemyBase::PlayDieReactionAnimation()
{
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	
	if (AnimInstance && DeathAnimMontage)
	{
		bIsStunned = false;
		if (AnimInstance->Montage_IsPlaying(StunMontage))
		{
			AnimInstance->Montage_Stop(0.1f,StunMontage);
		}
	
		AnimInstance->StopAllMontages(0.1f);
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &AEK_EnemyBase::OnDeathAnimationEnded);
		AnimInstance->Montage_Play(DeathAnimMontage, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, DeathAnimMontage);
	} 
}
void AEK_EnemyBase::OnDeathAnimationEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bInterrupted)
	{

		for (AActor* Actor : AttachedActors)
		{
			if (Actor)
			{
				Actor->Destroy();
			}
		}
		Destroy();
	}
}
#pragma endregion
#pragma region PlayStunMontage
void AEK_EnemyBase::PlayStunReactionAnimation() //just one time so I use Enemy_State_Stunned 
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance(); 
	if (AnimInstance && StunMontage)
	{
	
		EnemyStat->OnPoiseIsZero.Broadcast();
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &AEK_EnemyBase::OnStunAnimationEnded);
		AnimInstance->Montage_Play(StunMontage);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, StunMontage);
	}
}
void AEK_EnemyBase::OnStunAnimationEnded(UAnimMontage* Montage, bool bInterrupted)
{
	EnemyStat->ResetCurrentPoise();
	EnemyStat->OnStunAnimationEnd.Broadcast();
	bIsStunned = false;
}
#pragma endregion


AActor* AEK_EnemyBase::GetAttackTarget()
{
	return AttackTarget;
}
TObjectPtr<UEK_EnemyStatusComponent> AEK_EnemyBase::GetStatusComponent() 
{
	return EnemyStat;
}

void AEK_EnemyBase::SetAttackTarget(AActor* Actor)
{
	if (Actor)
	{
		AttackTarget = Actor;
	}
}

void AEK_EnemyBase::ReturnToInitializeLocation()
{
	AAIController* AIController = Cast<AAIController>(GetController()); 
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance&&AnimInstance->IsAnyMontagePlaying())
	{
		AnimInstance->StopAllMontages(0.1f);
	}

	if (AIController)
	{
		AIController->MoveToLocation(InitialLocation, AcceptanceRadius);
	}
}

void AEK_EnemyBase::StartBossBattle()
{
	UUISubsystem* UISystem = GetGameInstance()->GetSubsystem<UUISubsystem>();
	if (!UISystem) return;

	UISystem->SetWidgetVisibility(FEKGameplayTags::Get().UI_Widget_Game_BossBattle, ESlateVisibility::SelfHitTestInvisible);

	UUserWidget* widget = UISystem->GetWidget(FEKGameplayTags::Get().UI_Widget_Game_BossBattle);
	if (!widget) return;
	UWidget_BossBattle* userWidget = Cast<UWidget_BossBattle>(widget);
	if (!userWidget) return;
	userWidget->StartBossBattle(this);
}

FVector AEK_EnemyBase::GetInitializeLocation()
{
	return InitialLocation;
}

void AEK_EnemyBase::RemoveTimeslow()
{
	this->CustomTimeDilation = DefaultTimeDelayValue;
}

void AEK_EnemyBase::RemoveTimeslowTimer()
{
	this->CustomTimeDilation = TimeDelayValue;
	GetWorldTimerManager().SetTimer(RemoveTimeslowHandle, this, &ThisClass::RemoveTimeslow, TimeslowDuration, false);
}