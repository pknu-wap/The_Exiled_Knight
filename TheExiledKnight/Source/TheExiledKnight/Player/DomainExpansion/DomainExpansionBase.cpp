// Made by Somalia Pirate

#include "DomainExpansionBase.h"

ADomainExpansionBase::ADomainExpansionBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	RootComponent = SphereComponent;
}

void ADomainExpansionBase::BeginPlay()
{
	Super::BeginPlay();

	EKPlayer = Cast<AEKPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (EKPlayer)
	{
		EKPlayerController = Cast<AEKPlayerController>(EKPlayer->GetController());
	}

	if (DomainExpansionEffect)
	{
		SpawnedDomainExpansion = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DomainExpansionEffect, EKPlayer->GetActorLocation(), EKPlayer->GetActorRotation());
	}

	EKPlayer->EKPlayerStateContainer.AddTag(EKPlayerGameplayTags::EKPlayer_State_DomainExpansion);

	DomainExpansionTimer();

	RemoveEffectTimer();

	DomainExpansionCoolDownTimer();
}

void ADomainExpansionBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SpawnedDomainExpansion->SetWorldScale3D(FVector(DomainSize, DomainSize, DomainSize));
	SphereComponent->SetSphereRadius(320 * DomainSize);
}

void ADomainExpansionBase::DomainExpansion()
{
	if (DomainSize >= DomainMaxSize)
	{
		return;
	}

	DomainSize += DomainExpansionValue;
}

void ADomainExpansionBase::DomainExpansionTimer()
{
	GetWorldTimerManager().SetTimer(DomainExpansionTimeHandle, this, &ThisClass::DomainExpansion, DomainExpansionTime, true);
}

void ADomainExpansionBase::RemoveEffect()
{
	SpawnedDomainExpansion->DestroyComponent();
	SphereComponent->DestroyComponent();
}

void ADomainExpansionBase::RemoveEffectTimer()
{
	GetWorldTimerManager().SetTimer(RemoveEffectTimeHandle, this, &ThisClass::RemoveEffect, DomainDuration, false);
}

void ADomainExpansionBase::DomainExpansionCoolDown()
{
	EKPlayer->EKPlayerStateContainer.RemoveTag(EKPlayerGameplayTags::EKPlayer_State_DomainExpansion);
}

void ADomainExpansionBase::DomainExpansionCoolDownTimer()
{
	GetWorldTimerManager().SetTimer(CoolDownTimeHandle, this, &ThisClass::DomainExpansionCoolDown, DomainCoolDown, false);
}