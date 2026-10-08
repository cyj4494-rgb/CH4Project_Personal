// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CJPlayerState.h"

#include "Net/UnrealNetwork.h"

ACJPlayerState::ACJPlayerState() : PlayerNameString(TEXT("None")) , CurrentGuessCount(0), MaxGuessCount(3)
{
	bReplicates = true;
}

void ACJPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, PlayerNameString);
	DOREPLIFETIME(ThisClass, CurrentGuessCount);
	DOREPLIFETIME(ThisClass, MaxGuessCount);
}

FString ACJPlayerState::GetPlayerInfoString()
{
	FString PlayerInfoString = PlayerNameString + TEXT("(") + FString::FromInt(CurrentGuessCount) + TEXT("/") + FString::FromInt(MaxGuessCount) + TEXT(")");
	return PlayerInfoString;
}