// Made by Somalia Pirate

#include "DomainExpansionTimeSlow.h"

ADomainExpansionTimeSlow::ADomainExpansionTimeSlow()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ADomainExpansionTimeSlow::OnOverlapBegin);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ADomainExpansionTimeSlow::OnOverlapEnd);

	SetLifeSpan(DomainDuration + 1);
}

void ADomainExpansionTimeSlow::BeginPlay()
{
	Super::BeginPlay();

	EKPlayer->EKPlayerStateContainer.AddTag(EKPlayerGameplayTags::EKPlayer_State_DomainExpansion_TimeSlow);
}

void ADomainExpansionTimeSlow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector CapsuleLocation = SphereComponent->GetComponentLocation();
	FRotator CapsuleRotation = SphereComponent->GetComponentRotation();
	float CapsuleRadius = SphereComponent->GetScaledSphereRadius();

	TArray<FHitResult> HitResults;

	bool bIsHit = EKPlayer->GetWorld()->SweepMultiByChannel(
		HitResults,
		CapsuleLocation,
		CapsuleLocation,
		CapsuleRotation.Quaternion(),
		ECC_Pawn,
		FCollisionShape::MakeSphere(CapsuleRadius)
	);

	if (!bIsHit)
	{
		return;
	}

	for (auto& Hit : HitResults)
	{
		AEK_EnemyBase* HitEnemy = Cast<AEK_EnemyBase>(Hit.GetActor());

		if (HitEnemy)
		{
			HitEnemy->RemoveTimeslowTimer();
		}
	}
}

void ADomainExpansionTimeSlow::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OverlappedComponent->IsA(AEKPlayer::StaticClass()))
	{
		EKPlayer->EKPlayerStateContainer.AddTag(EKPlayerGameplayTags::EKPlayer_State_DomainExpansion_TimeSlow);
	}

	if (OverlappedComponent->IsA(AEK_EnemyBase::StaticClass()))
	{
		AEK_EnemyBase* HitEnemy = Cast<AEK_EnemyBase>(OverlappedComponent);
		HitEnemy->RemoveTimeslowTimer();
	}
}

void ADomainExpansionTimeSlow::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OverlappedComponent->IsA(AEKPlayer::StaticClass()))
	{
		EKPlayer->EKPlayerStateContainer.RemoveTag(EKPlayerGameplayTags::EKPlayer_State_DomainExpansion_TimeSlow);
	}

	if (OverlappedComponent->IsA(AEK_EnemyBase::StaticClass()))
	{
		AEK_EnemyBase* HitEnemy = Cast<AEK_EnemyBase>(OverlappedComponent);
		HitEnemy->RemoveTimeslowTimer();
	}
}