// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CJGameModeBase.h"

#include "Game/CJGameStateBase.h"

void ACJGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	ACJGameStateBase* CJGameStateBase = GetGameState<ACJGameStateBase>();
	if (IsValid(CJGameStateBase) == true) {
		CJGameStateBase->MulticastRPCBroadcastLoginMessage(TEXT("XXXXXX"));
	}
}
