// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CJGameModeBase.h"
#include "Player/CJPlayerState.h"
#include "Game/CJGameStateBase.h"
#include "Player/CJPlayerController.h"

#include "EngineUtils.h"
#include "TimerManager.h"

void ACJGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	ACJPlayerController* CJPlayerController = Cast<ACJPlayerController>(NewPlayer);
	if (IsValid(CJPlayerController) == true)
	{
		CJPlayerController->NotificationText = FText::FromString(TEXT("Connected to the game server."));

		AllPlayerControllers.Add(CJPlayerController);

		ACJPlayerState* CJPS = CJPlayerController->GetPlayerState<ACJPlayerState>();
		if (IsValid(CJPS) == true)
		{
			CJPS->PlayerNameString = TEXT("Player") + FString::FromInt(AllPlayerControllers.Num());

			ACJGameStateBase* CJGameStateBase = GetGameState<ACJGameStateBase>();
			if (IsValid(CJGameStateBase) == true)
			{
				CJGameStateBase->MulticastRPCBroadcastLoginMessage(CJPS->PlayerNameString);
			}
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
			if (UniqueDigits.Contains(C) == true)
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
	ACJPlayerState* CJPS = InChattingPlayerController->GetPlayerState<ACJPlayerState>();
	if (IsValid(CJPS) == false)
	{
		return;
	}

	FString GuessNumberString = InChatMessageString.TrimStartAndEnd();

	bool bIsGuessAttempt = false;
	for (TCHAR C : GuessNumberString)
	{
		if (FChar::IsDigit(C) == true)
		{
			bIsGuessAttempt = true;
			break;
		}
	}

	if (bIsGuessAttempt == false)
	{
		FString CombinedMessageString = CJPS->GetPlayerInfoString() + TEXT(": ") + GuessNumberString;
		for (TActorIterator<ACJPlayerController> It(GetWorld()); It; ++It)
		{
			ACJPlayerController* CJPlayerController = *It;
			if (IsValid(CJPlayerController) == true)
			{
				CJPlayerController->ClientRPCPrintChatMessageString(CombinedMessageString);
			}
		}
		return;
	}

	if (CJPS->CurrentGuessCount >= CJPS->MaxGuessCount)
	{
		InChattingPlayerController->ClientRPCPrintChatMessageString(TEXT("You have used all your chances."));
		return;
	}

	if (IsGuessNumberString(GuessNumberString) == false)
	{
		InChattingPlayerController->ClientRPCPrintChatMessageString(TEXT("Invalid input. Please enter again."));
		return;
	}

	FString JudgeResultString = JudgeResult(SecretNumberString, GuessNumberString);

	IncreaseGuessCount(InChattingPlayerController);

	FString CombinedMessageString = CJPS->GetPlayerInfoString() + TEXT(": ") + GuessNumberString + TEXT(" -> ") + JudgeResultString;
	for (TActorIterator<ACJPlayerController> It(GetWorld()); It; ++It)
	{
		ACJPlayerController* CJPlayerController = *It;
		if (IsValid(CJPlayerController) == true)
		{
			CJPlayerController->ClientRPCPrintChatMessageString(CombinedMessageString);
		}
	}

	int32 StrikeCount = FCString::Atoi(*JudgeResultString.Left(1));
	JudgeGame(InChattingPlayerController, StrikeCount);
}

void ACJGameModeBase::IncreaseGuessCount(ACJPlayerController* InChattingPlayerController)
{
	ACJPlayerState* CJPS = InChattingPlayerController->GetPlayerState<ACJPlayerState>();
	if (IsValid(CJPS) == true)
	{
		CJPS->CurrentGuessCount++;
	}
}

void ACJGameModeBase::ResetGame()
{
	SecretNumberString = GenerateSecretNumber();
	UE_LOG(LogTemp, Error, TEXT("%s"), *SecretNumberString)

		for (const auto& CJPlayerController : AllPlayerControllers)
		{
			ACJPlayerState* CJPS = CJPlayerController->GetPlayerState<ACJPlayerState>();
			if (IsValid(CJPS) == true)
			{
				CJPS->CurrentGuessCount = 0;
			}
			CJPlayerController->NotificationText = FText::FromString(TEXT("New game started!"));
		}
}

void ACJGameModeBase::JudgeGame(ACJPlayerController* InChattingPlayerController, int InStrikeCount)
{
	bool bIsGameOver = false;

	if (3 == InStrikeCount)
	{
		ACJPlayerState* CJPS = InChattingPlayerController->GetPlayerState<ACJPlayerState>();
		if (IsValid(CJPS) == true)
		{
			FString CombinedMessageString = CJPS->PlayerNameString + TEXT(" has won the game.");
			for (const auto& CJPlayerController : AllPlayerControllers)
			{
				CJPlayerController->NotificationText = FText::FromString(CombinedMessageString);
			}
			bIsGameOver = true;
		}
	}
	else
	{
		bool bIsDraw = true;
		for (const auto& CJPlayerController : AllPlayerControllers)
		{
			ACJPlayerState* CJPS = CJPlayerController->GetPlayerState<ACJPlayerState>();
			if (IsValid(CJPS) == true)
			{
				if (CJPS->CurrentGuessCount < CJPS->MaxGuessCount)
				{
					bIsDraw = false;
					break;
				}
			}
		}

		if (true == bIsDraw)
		{
			for (const auto& CJPlayerController : AllPlayerControllers)
			{
				CJPlayerController->NotificationText = FText::FromString(TEXT("Draw..."));
			}
			bIsGameOver = true;
		}
	}

	if (bIsGameOver == true)
	{
		FTimerHandle ResetTimerHandle;
		GetWorldTimerManager().SetTimer(ResetTimerHandle, this, &ACJGameModeBase::ResetGame, 3.0f, false);
	}
}