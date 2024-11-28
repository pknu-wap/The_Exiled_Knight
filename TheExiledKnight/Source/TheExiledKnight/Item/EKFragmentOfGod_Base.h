// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/EKItem_Base.h"
#include "EKFragmentOfGod_Base.generated.h"

/**
 * 
 */
UCLASS()
class THEEXILEDKNIGHT_API AEKFragmentOfGod_Base : public AEKItem_Base
{
	GENERATED_BODY()
	
public:
	AEKFragmentOfGod_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void UseItem(UWorld* WorldContext, float level = 1);

	virtual void Activate();

protected:
	float CoolDown = 0.0f;
	float Radius = 0.0f;
	float Duration = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USphereComponent* Collision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UNiagaraComponent* Niagara_FOG;
};
