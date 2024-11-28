// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/EKFragmentOfGod_Base.h"
#include "Subsystems/InventorySubsystem.h"
#include "Components/SphereComponent.h"

AEKFragmentOfGod_Base::AEKFragmentOfGod_Base()
{
}

void AEKFragmentOfGod_Base::BeginPlay()
{
	FFragmentOfGod* FOGInfo = GetWorld()->GetGameInstance<UInventorySubsystem>()->GetFOGInfo(ID);

	CoolDown = FOGInfo->CoolDown;
	Radius = FOGInfo->Radius;
	Duration = FOGInfo->Duration;

	Collision->SetSphereRadius(Radius);
	SetLifeSpan(Duration);
	/*
	쿨 다운은 여기가 아니라 플레이어 컨트롤러에서 키를 누를 때 적용되어야 함
	즉, 
	*/
}

void AEKFragmentOfGod_Base::Tick(float DeltaTime)
{
}

void AEKFragmentOfGod_Base::UseItem(UWorld* WorldContext, float level)
{
}

void AEKFragmentOfGod_Base::Activate()
{
	// 생츄어리 처럼 점점 커지는 코드 추가
}
