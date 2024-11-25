// Made by Somalia Pirate

#include "DomainExpansionHPPig.h"

ADomainExpansionHPPig::ADomainExpansionHPPig()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ADomainExpansionHPPig::OnOverlapBegin);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ADomainExpansionHPPig::OnOverlapEnd);
}

void ADomainExpansionHPPig::BeginPlay()
{
	Super::BeginPlay();
}

void ADomainExpansionHPPig::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADomainExpansionHPPig::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OverlappedComponent->IsA(AEKPlayer::StaticClass()))
	{
		EKPlayer->EKPlayerStateContainer.AddTag(EKPlayerGameplayTags::EKPlayer_State_DomainExpansion_HPPig);
	}
}

void ADomainExpansionHPPig::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OverlappedComponent->IsA(AEKPlayer::StaticClass()))
	{
		EKPlayer->EKPlayerStateContainer.RemoveTag(EKPlayerGameplayTags::EKPlayer_State_DomainExpansion_HPPig);
	}
}