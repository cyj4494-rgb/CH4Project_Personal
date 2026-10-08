// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CJGameModeBase.h"
#include "Player/CJPlayerState.h"
#include "Game/CJGameStateBase.h"
#include "Player/CJPlayerController.h"

#include "EngineUtils.h"
void ACJGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	/*ACJGameStateBase* CJGameStateBase = GetGameState<ACJGameStateBase>();
	if (IsValid(CJGameStateBase) == true) {
		CJGameStateBase->MulticastRPCBroadcastLoginMessage(TEXT("XXXXXX"));
	}

	ACJPlayerController* CJPlayerController = Cast<ACJPlayerController>(NewPlayer);
	if (IsValid(CJPlayerController) == true) {
		AllPlayerControllers.Add(CJPlayerController);
	}*/

	ACJPlayerController* CJPlayerController = Cast<ACJPlayerController>(NewPlayer);
	if (IsValid(CJPlayerController) == true)
	{
		AllPlayerControllers.Add(CJPlayerController);

		ACJPlayerState* CJPS = CJPlayerController->GetPlayerState<ACJPlayerState>();
		if (IsValid(CJPS) == true)
		{
			CJPS->PlayerNameString = TEXT("Player") + FString::FromInt(AllPlayerControllers.Num());
		}

		ACJGameStateBase* CJGameStateBase = GetGameState<ACJGameStateBase>();
		if (IsValid(CJGameStateBase) == true)
		{
			CJGameStateBase->MulticastRPCBroadcastLoginMessage(CJPS->PlayerNameString);
		}
	}
}

FString ACJGameModeBase::GenerateSecretNumber()
{
	TArray<int32> Numbers;
	for (int32 i = 1; i <= 9; ++i)
	{
		Numbers.Add(i);
	}

	FMath::RandInit(FDateTime::Now().GetTicks());
	Numbers = Numbers.FilterByPredicate([](int32 Num) { return Num > 0; });

	FString Result;
	for (int32 i = 0; i < 3; ++i)
	{
		int32 Index = FMath::RandRange(0, Numbers.Num() - 1);
		Result.Append(FString::FromInt(Numbers[Index]));
		Numbers.RemoveAt(Index);
	}

	return Result;
}

bool ACJGameModeBase::IsGuessNumberString(const FString& InNumberString)
{
	bool bCanPlay = false;

	do {

		if (InNumberString.Len() != 3)
		{
			break;
		}

		bool bIsUnique = true;
		TSet<TCHAR> UniqueDigits;
		for (TCHAR C : InNumberString)
		{
			if (FChar::IsDigit(C) == false || C == '0')
			{
				bIsUnique = false;
				break;
			}

			UniqueDigits.Add(C);
		}

		if (bIsUnique == false)
		{
			break;
		}

		bCanPlay = true;

	} while (false);

	return bCanPlay;
}

FString ACJGameModeBase::JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString)
{
	int32 StrikeCount = 0, BallCount = 0;

	for (int32 i = 0; i < 3; ++i)
	{
		if (InSecretNumberString[i] == InGuessNumberString[i])
		{
			StrikeCount++;
		}
		else
		{
			FString PlayerGuessChar = FString::Printf(TEXT("%c"), InGuessNumberString[i]);
			if (InSecretNumberString.Contains(PlayerGuessChar))
			{
				BallCount++;
			}
		}
	}

	if (StrikeCount == 0 && BallCount == 0)
	{
		return TEXT("OUT");
	}

	return FString::Printf(TEXT("%dS%dB"), StrikeCount, BallCount);
}

void ACJGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SecretNumberString = GenerateSecretNumber();
	UE_LOG(LogTemp, Error, TEXT("%s"), *SecretNumberString)
}

void ACJGameModeBase::PrintChatMessageString(ACJPlayerController* InChattingPlayerController, const FString& InChatMessageString)
{
	FString ChatMessageString = InChatMessageString;
	int Index = InChatMessageString.Len() - 3;
	FString GuessNumberString = InChatMessageString.RightChop(Index);
	if (IsGuessNumberString(GuessNumberString) == true)
	{
		FString JudgeResultString = JudgeResult(SecretNumberString, GuessNumberString);

		IncreaseGuessCount(InChattingPlayerController);

		for (TActorIterator<ACJPlayerController> It(GetWorld()); It; ++It)
		{
			ACJPlayerController* CJPlayerController = *It;
			if (IsValid(CJPlayerController) == true)
			{
				FString CombinedMessageString = InChatMessageString + TEXT(" -> ") + JudgeResultString;
				CJPlayerController->ClientRPCPrintChatMessageString(CombinedMessageString);
			}
		}
	}
	else
	{
		for (TActorIterator<ACJPlayerController> It(GetWorld()); It; ++It)
		{
			ACJPlayerController* CJPlayerController = *It;
			if (IsValid(CJPlayerController) == true)
			{
				CJPlayerController->ClientRPCPrintChatMessageString(InChatMessageString);
			}
		}
	}



}

void ACJGameModeBase::IncreaseGuessCount(ACJPlayerController* InChattingPlayerController)
{
	ACJPlayerState* CJPS = InChattingPlayerController->GetPlayerState<ACJPlayerState>();
	if (IsValid(CJPS) == true)
	{
		CJPS->CurrentGuessCount++;
	}
}
