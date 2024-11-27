// Made by Somalia Pirate

#pragma once

#include "Components/ActorComponent.h"
#include "EKPlayer.h"
#include "EKPlayerController.h"
#include "Player/GameInstance/EKPlayerGameInstance.h"
#include "EKPlayerStatusComponent.generated.h"

#pragma region Max Value

// Edit Player Basic Max Stat Here
#define PlayerMaxHp 1000
#define PlayerMaxMp 1000
#define PlayerMaxStamina 1000

// Edit Player Max Level Here
#define PlayerMaxLevel 594
#define PlayerMaxVitalityLevel 99
#define PlayerMaxMentalLevel 99
#define PlayerMaxEnduranceLevel 99
#define PlayerMaxStrengthLevel 99
#define PlayerMaxAbilityLevel 99
#define PlayerMaxInteligenceLevel 99

#pragma endregion

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHPUpdated, int32, MaxHP, int32, CurrentHP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStaminaUpdated, int32, MaxStamina, int32, CurrentStamina);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMPUpdated, int32, MaxMP, int32, CurrentMP);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UEKPlayerStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEKPlayerStatusComponent();

public:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void RestoreState();

	UFUNCTION(BlueprintCallable)
	void Recalculate_Status();

	void Calculate_BasicStatus();
	void Calculate_NormalStatus();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetCalculatedHP(int InVitality);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetCalculatedMP(int InMental);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetCalculatedStamina(int InEndurance);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetCalculatedATK(int InStrength, int InAbility, int InIntelligence);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetCalculatedDEF(int InEndurance, int InAbility);

#pragma region Status Function

public:
	int32 GetMaxHp() { return MaxHp; }
	int32 GetHp() { return Hp; }
	int32 GetMaxMp() { return MaxMp; }
	int32 GetMp() { return Mp; }
	int32 GetMaxStamina() { return MaxStamina; }
	int32 GetStamina() { return Stamina; }

	int32 GetVitality() { return Vitality; }
	int32 GetMental() { return Mental; }
	int32 GetEndurance() { return Endurance; }
	int32 GetStrength() { return Strength; }
	int32 GetAbility() { return Ability; }
	int32 GetIntelligence() { return Intelligence; }

	int32 GetBaseVitality() { return BaseVitality; }
	int32 GetBaseMental() { return BaseMental; }
	int32 GetBaseEndurance() { return BaseEndurance; }
	int32 GetBaseStrength() { return BaseStrength; }
	int32 GetBaseAbility() { return BaseAbility; }
	int32 GetBaseIntelligence() { return BaseIntelligence; }

	void SetStatusHandle();

	void SetMaxHp();
	void SetHp(int32 SetData);
	void SetMaxMp();
	void SetMp(int32 SetData);
	void SetMaxStamina();
	void SetStamina(int32 SetData);

public:
	float GetPlayerDefaultDamage() { return DefaultDamage; }
	void SetPlayerDefaultDamage();

	float GetPlayerFinalDamage() { return FinalDamage; }
	void SetPlayerFinalDamage();

	void LevelUp(uint8 IncreaseLevel);
	void LevelUpVitality(uint8 IncreaseLevel);
	void LevelUpMental(uint8 IncreaseLevel);
	void LevelUpEndurance(uint8 IncreaseLevel);
	void LevelUpStrength(uint8 IncreaseLevel);
	void LevelUpAbility(uint8 IncreaseLevel);
	void LevelUpIntelligence(uint8 IncreaseLevel);

#pragma endregion

public:
	bool bCanStaminaRecovery = true;

protected:
	UPROPERTY()
	AEKPlayer* EKPlayer;

	UPROPERTY()
	AEKPlayerController* EKPlayerController;

	UPROPERTY()
	UEKPlayerGameInstance* EKPlayerGameInstance;

	UPROPERTY()
	FEKPlayerLevel EKPlayerLevelData;

	UPROPERTY()
	FEKPlayerStatus EKPlayerStatusData;

#pragma region Basic Status

protected:
	// Basic status
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Statment|Basic")
	int MaxHp = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Statment|Basic")
	int Hp = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Statment|Basic")
	int MaxMp = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Statment|Basic")
	int Mp = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Statment|Basic")
	int MaxStamina = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Statment|Basic")
	int Stamina = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Statment|Basic")
	int ATK;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Statment|Basic")
	int DEF;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Statment|Basic")
	int32 CurrentAstral;

	UPROPERTY(VisibleAnywhere, Category = "Statment|Basic")
	float DefaultDamage;

	UPROPERTY(VisibleAnywhere, Category = "Statment|Basic")
	float FinalDamage;

#pragma endregion

#pragma region Upgrade Status

protected:
	// Upgrade status
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Statment|Upgrade")
	int32 Level;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Statment|Upgrade")
	int32 Vitality;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Statment|Upgrade")
	int32 BaseVitality;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Statment|Upgrade")
	int32 Mental;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Statment|Upgrade")
	int32 BaseMental;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Statment|Upgrade")
	int32 Endurance;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Statment|Upgrade")
	int32 BaseEndurance;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Statment|Upgrade")
	int32 Strength;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Statment|Upgrade")
	int32 BaseStrength;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Statment|Upgrade")
	int32 Ability;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Statment|Upgrade")
	int32 BaseAbility;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Statment|Upgrade")
	int32 Intelligence;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Statment|Upgrade")
	int32 BaseIntelligence;

#pragma endregion

public:
	UPROPERTY(BlueprintAssignable)
	FOnHPUpdated Delegate_HPUpdated;

	UPROPERTY(BlueprintAssignable)
	FOnMPUpdated Delegate_MPUpdated;

	UPROPERTY(BlueprintAssignable)
	FOnStaminaUpdated Delegate_StaminaUpdated;
};
