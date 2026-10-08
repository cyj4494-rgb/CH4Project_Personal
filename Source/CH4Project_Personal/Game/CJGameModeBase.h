// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CJGameModeBase.generated.h"

class ACJPlayerController;
/**
 * 
 */
UCLASS()
class CH4PROJECT_PERSONAL_API ACJGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void OnPostLogin(AController* NewPlayer) override;
	
	FString GenerateSecretNumber();
	bool IsGuessNumberString(const FString& InNumberString);
	FString JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString);

	virtual void BeginPlay() override;

	void PrintChatMessageString(ACJPlayerController* InChattingPlayerController, const FString& InChatMessageString); 

	void IncreaseGuessCount(ACJPlayerController* InChattingPlayerController);

protected:
	FString SecretNumberString;

	TArray<TObjectPtr<ACJPlayerController>> AllPlayerControllers;
};
