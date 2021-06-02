// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"
#include "PuzzlePlatformGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERBASICS_API UPuzzlePlatformGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()
	
public:
	UPuzzlePlatformGameInstance();
	
	virtual void Init();

	UFUNCTION(Exec)
	void Host();

	UFUNCTION(Exec)
		void Join(const FString& ip);

	UFUNCTION(Exec)
		void Leave();

	UFUNCTION(BlueprintCallable)
		void LoadMainMenu();

	void LoadPauseMenu();

private:

	class UMainMenuUserWidget* Menu;

	class UPauseMenuUserWidget* PMenu;

	UPROPERTY()
		TSubclassOf<UUserWidget> WBPMainMenuClass;

	UPROPERTY()
		TSubclassOf<UUserWidget> WBPPauseMenuClass;
};
