// Made by Somalia Pirate

#pragma once

#include "Player/DomainExpansion/DomainExpansionBase.h"
#include "DomainExpansionHPPig.generated.h"

UCLASS()
class THEEXILEDKNIGHT_API ADomainExpansionHPPig : public ADomainExpansionBase
{
	GENERATED_BODY()

public:
	ADomainExpansionHPPig();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};