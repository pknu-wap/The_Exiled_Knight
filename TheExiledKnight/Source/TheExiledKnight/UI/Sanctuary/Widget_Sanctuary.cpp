// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Sanctuary/Widget_Sanctuary.h"

void UWidget_Sanctuary::CountUpgrade_Success()
{
	PlayAnimation(Potion_CountUpgrade_Success);
}

void UWidget_Sanctuary::CountUpgrade_Failed()
{
	PlayAnimation(Potion_CountUpgrade_Failed);
}

void UWidget_Sanctuary::RateUpgrade_Success()
{
	PlayAnimation(Potion_Upgrade_Success);
}

void UWidget_Sanctuary::RateUpgrade_Failed()
{
	PlayAnimation(Potion_Upgrade_Failed);
}
