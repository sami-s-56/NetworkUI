// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERBASICS_API UMainMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	virtual bool Initialize();	//To do stuff when user widget is initialized

	UFUNCTION()
	void OnHostButton();

	UFUNCTION()
		void OnJoinButton();

	UFUNCTION()
		void OnBackButton();

	UFUNCTION()
		void OnJoinGameButton();

	UFUNCTION()
	void OnQuitButton();

private:

	class IMenuInterface* menuInterface;

	UPROPERTY(Meta = (BindWidget))
		class UButton* HostButton;

	UPROPERTY(Meta = (BindWidget))
		class UButton* JoinButton;

	UPROPERTY(Meta = (BindWidget))
		class UButton* BackButton;

	UPROPERTY(Meta = (BindWidget))
		class UButton* GameJoinButton_1;

	UPROPERTY(Meta = (BindWidget))
		class UButton* QuitButton;

	UPROPERTY(Meta = (BindWidget))
		class UEditableTextBox* IPAddBox;

	UPROPERTY(Meta = (BindWidget))
		class UWidgetSwitcher* MenuSwitcher;
	

public:

	void SetMenuInterface(IMenuInterface* interface);

	void LoadMenu();

	void RemoveMenu();

};
