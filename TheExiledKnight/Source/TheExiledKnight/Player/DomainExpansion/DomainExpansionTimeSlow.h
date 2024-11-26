// Made by Somalia Pirate

#pragma once

#include "Player/DomainExpansion/DomainExpansionBase.h"
#include "DomainExpansionTimeSlow.generated.h"

UCLASS()
class THEEXILEDKNIGHT_API ADomainExpansionTimeSlow : public ADomainExpansionBase
{
	GENERATED_BODY()

public:
	ADomainExpansionTimeSlow();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};