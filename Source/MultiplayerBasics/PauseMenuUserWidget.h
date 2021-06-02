// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/MenuInterface.h"
#include "PauseMenuUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERBASICS_API UPauseMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:

	class IMenuInterface* menuInterface;

	UPROPERTY(Meta = (BindWidget))
		class UButton* ResumeButton;

	UPROPERTY(Meta = (BindWidget))
		class UButton* LeaveButton;

protected:
	virtual bool Initialize();	//To do stuff when user widget is initialized

	UFUNCTION()
		void OnResumeButton();

	UFUNCTION()
		void OnLeaveButton();

public:

	void SetMenuInterface(IMenuInterface* interface) { menuInterface = interface; }

	void LoadMenu();

	void RemoveMenu();

	
};
