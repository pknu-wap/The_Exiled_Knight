// Fill out your copyright notice in the Description page of Project Settings.


#include "EKEnemyAIController.h"
#include"Perception/AIPerceptionComponent.h"
#include"Perception/AISenseConfig.h"
#include"Perception/AIPerceptionTypes.h"
#include "Enemy/EK_EnemyStatusComponent.h"
#include"Enemy/EK_EnemyBase.h"


AEKEnemyAIController::AEKEnemyAIController()
{
 
#pragma region AIPerception
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SetPerceptionComponent(*AIPerception);

#pragma region Sight
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = SightRadius;
	SightConfig->LoseSightRadius = LostSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = DefaultPeripheralVisionAngle; 
	SightConfig->SetMaxAge(DefaultSightMaxAge);
	SightConfig->AutoSuccessRangeFromLastSeenLocation = -1.f;

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	AIPerception->ConfigureSense(*SightConfig);
	AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());
#pragma endregion
#pragma region Hearing
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	HearingConfig->HearingRange = HearingRange;
	HearingConfig->SetMaxAge(DefaultSightMaxAge);

	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
	AIPerception->ConfigureSense(*HearingConfig);

#pragma endregion

	DamageSenseConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageSenseConfig"));

	AIPerception->ConfigureSense(*DamageSenseConfig);
#pragma endregion
	 

}


void AEKEnemyAIController::SettingRange(float SightRange, float HearingDis)
{
	if (!SightConfig)return;
	SightConfig->SightRadius = SightRange;
	SightConfig->LoseSightRadius = 2 * SightRange;
	AIPerception->ConfigureSense(*SightConfig);

	if (!HearingConfig)return;
	HearingConfig->HearingRange = HearingDis;
	AIPerception->ConfigureSense(*HearingConfig);

}






	
	



